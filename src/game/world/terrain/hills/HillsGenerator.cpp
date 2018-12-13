#include "game/world/terrain/hills/HillsGenerator.h"

HillsGenerator::HillsGenerator(HillsShader &shaders, const map2D_f &waterMap)
  :
    Generator(),
    culledBuffers(VAO | VBO | TFBO),
    shaders(shaders),
    waterMap(waterMap)
{
  randomizer.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

void HillsGenerator::setup()
{
  generateMap(12, HILL_DENSITY::HILLS_DENSE);
  generateMap(6, HILL_DENSITY::HILLS_THIN);
  smoothMapSinks();
  compressMap(0.00f, 1.33f); //compress entire range
  compressMap(0.66f * maxHeight, 2.0f);
  removeMapPlateaus(1.0f);
  for (unsigned int i = 0; i < 4; i++)
    {
      smoothMapHeightChunks(map, 0.6f, 0.05f, 0.05f);
    }
  createTilesAndBufferData();
}

void HillsGenerator::createTilesAndBufferData()
{
  tiles.clear();
  for (unsigned int y = 1; y < map.size(); y++)
    {
      for (unsigned int x = 1; x < map[0].size(); x++)
        {
          if (map[y][x] == TILE_NO_RENDER_VALUE)
            continue;
          if (map[y][x] != 0 || map[y-1][x] != 0 || map[y][x-1] != 0 || map[y-1][x-1] != 0)
            {
              float ll = map[y][x-1] + HILLS_OFFSET_Y;
              float lr = map[y][x] + HILLS_OFFSET_Y;
              float ur = map[y-1][x] + HILLS_OFFSET_Y;
              float ul = map[y-1][x-1] + HILLS_OFFSET_Y;
              tiles.emplace_back(x, y, ll, lr, ur, ul);
            }
        }
    }
  smoothNormals(map, normalMap);
  tiles.shrink_to_fit();
  fillBufferData();
}

void HillsGenerator::fillBufferData()
{
  const size_t VERTEX_DATA_LENGTH = tiles.size() * 32;
  const size_t INDICES_DATA_LENGTH = tiles.size() * 6;
  size_t indicesBufferIndex = 0;
  std::unique_ptr<GLfloat[]> vertices(new GLfloat[VERTEX_DATA_LENGTH]);
  std::unique_ptr<GLuint[]> indices(new GLuint[INDICES_DATA_LENGTH]);
  for (unsigned int c = 0; c < tiles.size(); c++)
    {
      TerrainTile& tile = tiles[c];
      bool verticesCrossed = false;
      if (tile.lowRight < tile.upperLeft || tile.upperLeft < tile.lowRight)
        verticesCrossed = true;

      float texCoordXOffset = (tile.mapX % 2) * 0.5f;
      float texCoordYOffset = ((WORLD_HEIGHT - tile.mapY) % 2) * 0.5f;
      int x = tile.mapX, y = tile.mapY;

      HillVertex lowLeft(glm::vec3(x - 1, tile.lowLeft, y),
                         glm::vec2(texCoordXOffset, texCoordYOffset),
                         normalMap[y][x-1]);
      HillVertex lowRight(glm::vec3(x, tile.lowRight, y),
                          glm::vec2(0.5f + texCoordXOffset, texCoordYOffset),
                          normalMap[y][x]);
      HillVertex upRight(glm::vec3(x, tile.upperRight, y - 1),
                         glm::vec2(0.5f + texCoordXOffset, 0.5f + texCoordYOffset),
                         normalMap[y-1][x]);
      HillVertex upLeft(glm::vec3(x - 1, tile.upperLeft, y - 1),
                        glm::vec2(texCoordXOffset, 0.5f + texCoordYOffset),
                        normalMap[y-1][x-1]);

      int vertexBufferOffset = c * 32;
      bufferVertex(vertices.get(), vertexBufferOffset,    lowLeft);
      bufferVertex(vertices.get(), vertexBufferOffset+8,  lowRight);
      bufferVertex(vertices.get(), vertexBufferOffset+16, upRight);
      bufferVertex(vertices.get(), vertexBufferOffset+24, upLeft);

      GLuint indicesBufferBaseVertex = c * 4;
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + (verticesCrossed ? 3 : 0);
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + (verticesCrossed ? 0 : 1);
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + (verticesCrossed ? 1 : 2);
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + (verticesCrossed ? 1 : 2);
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + (verticesCrossed ? 2 : 3);
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + (verticesCrossed ? 3 : 0);
    }
  basicGLBuffers.bind(VAO | VBO | EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * VERTEX_DATA_LENGTH, vertices.get(), GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * INDICES_DATA_LENGTH, indices.get(), GL_STATIC_DRAW);
  setupGLBufferAttributes();

  if (culledBuffers.get(VAO) != 0)
    culledBuffers.deleteBuffers();
  culledBuffers.create(VAO | VBO | TFBO);
  culledBuffers.bind(VAO | VBO | TFBO);
  glNamedBufferStorage(culledBuffers.get(VBO), VERTEX_DATA_LENGTH * sizeof(GLfloat), 0, GL_NONE);
  setupGLBufferAttributes();
  shaders.setupCulling();
  glTransformFeedbackBufferBase(culledBuffers.get(TFBO), 0, culledBuffers.get(VBO));
  resetAllGLBuffers();
}

void HillsGenerator::generateMap(int cycles, float density)
{
  generateKernel(cycles, density);
  fattenKernel(cycles);
}

void HillsGenerator::generateKernel(int cycles, float density)
{
  for (int y = 1; y < WORLD_HEIGHT - 1; y++)
    {
      for (int x = 1; x < WORLD_WIDTH - 1; x++)
        {
          if (rand() % (int)density == 0 && !hasWaterNearby(x, y, cycles + 3))
            map[y][x] += 1.0f;
        }
    }
  maxHeight = 1.0f;
}

void HillsGenerator::fattenKernel(int cycles)
{
  std::uniform_real_distribution<float> heightDistribution(0.3f, 0.8f);
  for (int cycle = 1; cycle < cycles; cycle++)
    {
      for (int y = cycle; y < WORLD_HEIGHT - cycle; y++)
        {
          for (int x = cycle; x < WORLD_WIDTH - cycle; x++)
            {
              if (y >= WORLD_HEIGHT)
                break;
              if (rand() % (cycle+1) == cycle && (map[y][x] != 0))
                {
                  int left = (x-cycle <= cycle ? cycle : x-cycle);
                  int right = (x+cycle >= WORLD_WIDTH-cycle-1 ? WORLD_WIDTH-cycle-1 : x+cycle);
                  int top = (y-cycle <= cycle ? cycle : y-cycle);
                  int bottom = (y+cycle >= WORLD_HEIGHT-cycle-1 ? WORLD_HEIGHT-cycle-1 : y+cycle);
                  for (int y2 = top; y2 <= bottom; y2++)
                    {
                      for (int x2 = left; x2 <= right; x2++)
                        {
                          if (rand() % (cycle + 2) > 1)
                            {
                              if (hasWaterNearby(x2, y2, 8))
                                {
                                  --bottom;
                                  --right;
                                  continue;
                                }
                              map[y2][x2] += 1.0f - 0.05f * cycle + (heightDistribution(randomizer));
                              if (map[y2][x2] > maxHeight)
                                maxHeight = map[y2][x2];
                            }
                        }
                    }
                  x += cycle;
                  y += cycle;
                }
            }
        }
        removeOrphanHills();
    }
}

void HillsGenerator::bufferVertex(GLfloat* buffer, int offset, HillVertex vertex)
{
  buffer[offset] =   vertex.posX;
  buffer[offset+1] = vertex.posY;
  buffer[offset+2] = vertex.posZ;
  buffer[offset+3] = vertex.texCoordX;
  buffer[offset+4] = vertex.texCoordY;
  buffer[offset+5] = vertex.normalX;
  buffer[offset+6] = vertex.normalY;
  buffer[offset+7] = vertex.normalZ;
}

void HillsGenerator::setupGLBufferAttributes()
{
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
}

bool HillsGenerator::hasWaterNearby(int x, int y, int radius)
{
  int xLeft = (x-radius <= 0 ? 0 : x-radius);
  int xRight = (x+radius >= WORLD_WIDTH ? WORLD_WIDTH : x+radius);
  int yTop = (y-radius <= 0 ? 0 : y-radius);
  int yBottom = (y+radius >= WORLD_HEIGHT ? WORLD_HEIGHT : y+radius);
  for (int x1 = xLeft; x1 <= xRight; x1++)
    {
      for (int y1 = yTop; y1 <= yBottom; y1++)
        {
          if (waterMap[y1][x1] != 0)
            return true;
        }
    }
  return false;
}

void HillsGenerator::compressMap(float thresholdValue, float ratio)
{
  for (auto& row : map)
    for (auto& height : row)
      {
        if (height < thresholdValue)
          continue;
        height = thresholdValue + (height - thresholdValue) / ratio;
      }
  updateMaxHeight();
}

void HillsGenerator::updateMaxHeight()
{
  float newMaxHeight = 0.0f;
  for (int y = 1; y < WORLD_HEIGHT - 1; y++)
    {
      for (int x = 1; x < WORLD_WIDTH - 1; x++)
        {
          if (map[y][x] > newMaxHeight)
            newMaxHeight = map[y][x];
        }
    }
  maxHeight = newMaxHeight;
}

void HillsGenerator::removeMapPlateaus(float plateauHeight)
{
  unsigned int yTop, yBottom, xLeft, xRight;
  for (unsigned int y = 1; y < WORLD_HEIGHT - 1; y++)
    {
      for (unsigned int x = 1; x < WORLD_WIDTH - 1; x++)
        {
          if (map[y][x] == 0)
            continue;
          unsigned int plateauHeightNeighbourTiles = 0, neighboursLimit = 6;
          yTop = y - 1;
          yBottom = y + 1;
          xLeft = x - 1;
          xRight = x + 1;
          for (auto y1 = yTop; y1 <= yBottom; y1++)
            {
              for (auto x1 = xLeft; x1 <= xRight; x1++)
                {
                  if (y1 == y && x1 == x)
                    continue;
                  if (map[y1][x1] <= plateauHeight)
                    ++plateauHeightNeighbourTiles;
                }
            }
          if (plateauHeightNeighbourTiles >= neighboursLimit)
            map[y][x] = 0;
        }
    }
}

void HillsGenerator::removeOrphanHills()
{
  for (int y = 1; y < WORLD_HEIGHT; y++)
    {
      for (int x = 1; x < WORLD_WIDTH; x++)
        {
          if (map[y][x] != 0.0f &&
              map[y-1][x-1] == 0 &&
              map[y-1][x] == 0 &&
              map[y-1][x+1] == 0 &&
              map[y][x-1] == 0 &&
              map[y][x+1] == 0 &&
              map[y+1][x-1] == 0 &&
              map[y+1][x] == 0 &&
              map[y+1][x+1] == 0)
            map[y][x] = 0.0f;
        }
    }
}

void HillsGenerator::smoothMapSinks()
{
  for (unsigned int y = 1; y < WORLD_HEIGHT; y++)
    {
      for (unsigned int x = 1; x < WORLD_WIDTH; x++)
        {
          unsigned int higherNeighbours = 0;
          for (int yOffset = -1; yOffset <= 1; yOffset++)
            {
              for (int xOffset = -1; xOffset <= 1; xOffset++)
                {
                  if (yOffset == 0 && xOffset == 0)
                    continue;
                  higherNeighbours += (map[y][x] < map[y+yOffset][x+xOffset] ? 1 : 0);
                }
            }
          if (higherNeighbours >= 6)
            {
              float avgHeight = map[y-1][x-1]
                  +map[y-1][x]
                  +map[y-1][x+1]
                  +map[y][x-1]
                  +map[y][x+1]
                  +map[y+1][x-1]
                  +map[y+1][x]
                  +map[y+1][x+1];
              avgHeight /= 9;
              map[y][x] = avgHeight;
            }
        }
    }
}

HillsGenerator::HillVertex::HillVertex(glm::vec3 pos, glm::vec2 texCoords, glm::vec3 normal)
  :
    posX(pos.x - HALF_WORLD_WIDTH), posY(pos.y), posZ(pos.z - HALF_WORLD_HEIGHT),
    texCoordX(texCoords.x), texCoordY(texCoords.y),
    normalX(normal.x), normalY(normal.y), normalZ(normal.z)
{}
