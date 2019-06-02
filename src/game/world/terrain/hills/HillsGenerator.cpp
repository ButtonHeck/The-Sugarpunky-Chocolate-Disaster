#include "HillsGenerator.h"

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
  compressMap(0.66f * maxHeight, 2.0f); //compress top-most peaks
  removeHubbles(1.0f);
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
  generateTangentMap();
  generateBitangentMap();
  tiles.shrink_to_fit();
  fillBufferData();
}

void HillsGenerator::fillBufferData()
{
  const size_t VERTEX_DATA_LENGTH = tiles.size() * UNIQUE_VERTICES_PER_TILE * HillVertex::NUMBER_OF_ELEMENTS;
  const size_t INDICES_DATA_LENGTH = tiles.size() * VERTICES_PER_QUAD;
  size_t indicesBufferIndex = 0;
  std::unique_ptr<GLfloat[]> vertices(new GLfloat[VERTEX_DATA_LENGTH]);
  std::unique_ptr<GLuint[]> indices(new GLuint[INDICES_DATA_LENGTH]);
  for (unsigned int tileIndex = 0; tileIndex < tiles.size(); tileIndex++)
    {
      TerrainTile& tile = tiles[tileIndex];
      bool verticesCrossed = false;
      if (tile.lowRight < tile.upperLeft || tile.upperLeft < tile.lowRight)
        verticesCrossed = true;

      //map hill texture to cover HILL_TILING_PER_TEXTURE_QUAD^2 tiles instead of one
      float tilingSizeReciprocal = 1.0f / HILL_TILING_PER_TEXTURE_QUAD;
      float texCoordXOffset = (tile.mapX % HILL_TILING_PER_TEXTURE_QUAD) * tilingSizeReciprocal;
      float texCoordYOffset = ((WORLD_HEIGHT - tile.mapY) % HILL_TILING_PER_TEXTURE_QUAD) * tilingSizeReciprocal;
      int x = tile.mapX, y = tile.mapY;

      HillVertex lowLeft(glm::vec3(x - 1, tile.lowLeft, y),
                         glm::vec2(texCoordXOffset, texCoordYOffset),
                         normalMap[y][x-1],
                         tangentMap[y][x-1],
                         bitangentMap[y][x-1]);
      HillVertex lowRight(glm::vec3(x, tile.lowRight, y),
                          glm::vec2(tilingSizeReciprocal + texCoordXOffset, texCoordYOffset),
                          normalMap[y][x],
                          tangentMap[y][x],
                          bitangentMap[y][x]);
      HillVertex upRight(glm::vec3(x, tile.upperRight, y - 1),
                         glm::vec2(tilingSizeReciprocal + texCoordXOffset, tilingSizeReciprocal + texCoordYOffset),
                         normalMap[y-1][x],
                         tangentMap[y-1][x],
                         bitangentMap[y-1][x]);
      HillVertex upLeft(glm::vec3(x - 1, tile.upperLeft, y - 1),
                        glm::vec2(texCoordXOffset, tilingSizeReciprocal + texCoordYOffset),
                        normalMap[y-1][x-1],
                        tangentMap[y-1][x-1],
                        bitangentMap[y-1][x-1]);

      int vertexBufferOffset = tileIndex * UNIQUE_VERTICES_PER_TILE * HillVertex::NUMBER_OF_ELEMENTS;
      bufferVertex(vertices.get(), vertexBufferOffset,    lowLeft);
      bufferVertex(vertices.get(), vertexBufferOffset+14, lowRight);
      bufferVertex(vertices.get(), vertexBufferOffset+28, upRight);
      bufferVertex(vertices.get(), vertexBufferOffset+42, upLeft);

      GLuint indicesBufferBaseVertex = tileIndex * UNIQUE_VERTICES_PER_TILE;
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
  /* after transform feedback we have approximately 1.5 times more data to buffer in GL_ARRAY_BUFFER
   * (6 indices of 4 vertices transforms into 6 vertices, 2 of them have the same attributes)
   * because of duplication of vertices that have same attributes but different indices
   */
  glNamedBufferStorage(culledBuffers.get(VBO), VERTEX_DATA_LENGTH * sizeof(GLfloat) * 2, 0, GL_NONE);
  setupGLBufferAttributes();
  shaders.setupCulling();
  glTransformFeedbackBufferBase(culledBuffers.get(TFBO), 0, culledBuffers.get(VBO));
  BufferCollection::bindZero(VAO | VBO | EBO);
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
  std::uniform_real_distribution<float> heightDistribution(MIN_FATTENING_HEIGHT, MAX_FATTENING_HEIGHT);
  for (int cycle = 1; cycle < cycles; cycle++)
    {
      for (int startY = cycle; startY < WORLD_HEIGHT - cycle; startY++)
        {
          for (int startX = cycle; startX < WORLD_WIDTH - cycle; startX++)
            {
              if (startY >= WORLD_HEIGHT)
                break;
              if (rand() % (cycle+1) == cycle && (map[startY][startX] != 0))
                {
                  int left = (startX-cycle <= cycle ? cycle : startX-cycle);
                  int right = (startX+cycle >= WORLD_WIDTH-cycle-1 ? WORLD_WIDTH-cycle-1 : startX+cycle);
                  int top = (startY-cycle <= cycle ? cycle : startY-cycle);
                  int bottom = (startY+cycle >= WORLD_HEIGHT-cycle-1 ? WORLD_HEIGHT-cycle-1 : startY+cycle);
                  for (int y = top; y <= bottom; y++)
                    {
                      for (int x = left; x <= right; x++)
                        {
                          if (rand() % (cycle + 2) > 1)
                            {
                              //shortening the fattening borders to prevent hills generating over the shore and water
                              if (hasWaterNearby(x, y, 3 + SHORE_SMOOTH_CYCLES))
                                {
                                  --bottom;
                                  --right;
                                  continue;
                                }
                              map[y][x] += 1.0f - CYCLE_FATTENING_DAMPING_FACTOR * cycle + (heightDistribution(randomizer));
                              if (map[y][x] > maxHeight)
                                maxHeight = map[y][x];
                            }
                        }
                    }
                  //comment these two lines to see the Chocolate Grand Canyon
                  startX += cycle;
                  startY += cycle;
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
  buffer[offset+8] = vertex.tangentX;
  buffer[offset+9] = vertex.tangentY;
  buffer[offset+10] = vertex.tangentZ;
  buffer[offset+11] = vertex.bitangentX;
  buffer[offset+12] = vertex.bitangentY;
  buffer[offset+13] = vertex.bitangentZ;
}

void HillsGenerator::setupGLBufferAttributes()
{
  const size_t SIZE_OF_HILL_VERTEX =  HillVertex::NUMBER_OF_ELEMENTS * sizeof(GLfloat);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SIZE_OF_HILL_VERTEX, 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, SIZE_OF_HILL_VERTEX, (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, SIZE_OF_HILL_VERTEX, (void*)(5 * sizeof(GLfloat)));
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, SIZE_OF_HILL_VERTEX, (void*)(8 * sizeof(GLfloat)));
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, SIZE_OF_HILL_VERTEX, (void*)(11 * sizeof(GLfloat)));
}

bool HillsGenerator::hasWaterNearby(int centerX, int centerY, int radius)
{
  int xLeft = (centerX-radius <= 0 ? 0 : centerX-radius);
  int xRight = (centerX+radius >= WORLD_WIDTH ? WORLD_WIDTH : centerX+radius);
  int yTop = (centerY-radius <= 0 ? 0 : centerY-radius);
  int yBottom = (centerY+radius >= WORLD_HEIGHT ? WORLD_HEIGHT : centerY+radius);
  for (int x = xLeft; x <= xRight; x++)
    {
      for (int y = yTop; y <= yBottom; y++)
        {
          if (waterMap[y][x] != 0)
            return true;
        }
    }
  return false;
}

void HillsGenerator::compressMap(float thresholdAbsValue, float ratio)
{
  for (auto& row : map)
    for (auto& height : row)
      {
        if (height < thresholdAbsValue)
          continue;
        height = thresholdAbsValue + (height - thresholdAbsValue) / ratio;
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

void HillsGenerator::removeHubbles(float hubbleHeight)
{
  unsigned int yTop, yBottom, xLeft, xRight;
  for (unsigned int y = 1; y < WORLD_HEIGHT - 1; y++)
    {
      for (unsigned int x = 1; x < WORLD_WIDTH - 1; x++)
        {
          if (map[y][x] == 0)
            continue;
          unsigned int plateauHeightNeighbourTiles = 0;
          const unsigned int NEIGHBOURS_LIMIT = 6;
          yTop = y - 1;
          yBottom = y + 1;
          xLeft = x - 1;
          xRight = x + 1;
          for (auto nY = yTop; nY <= yBottom; nY++)
            {
              for (auto nX = xLeft; nX <= xRight; nX++)
                {
                  if (nY == y && nX == x) //don't need to check itself
                    continue;
                  if (map[nY][nX] <= hubbleHeight)
                    ++plateauHeightNeighbourTiles;
                }
            }
          if (plateauHeightNeighbourTiles >= NEIGHBOURS_LIMIT)
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
          const unsigned int NEIGHBOURS_LIMIT = 6;
          for (int yOffset = -1; yOffset <= 1; yOffset++)
            {
              for (int xOffset = -1; xOffset <= 1; xOffset++)
                {
                  if (yOffset == 0 && xOffset == 0) //don't need to check itself
                    continue;
                  higherNeighbours += (map[y][x] < map[y+yOffset][x+xOffset] ? 1 : 0);
                }
            }
          if (higherNeighbours >= NEIGHBOURS_LIMIT)
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

void HillsGenerator::generateTangentMap()
{
  using glm::vec3;
  tangentMap.clear();
  tangentMap.reserve(WORLD_HEIGHT + 1);
  for (size_t row = 0; row < WORLD_HEIGHT + 1; row++)
    {
      vec3 defaultTangent(1.0f, 0.0f, 0.0f);
      std::vector<vec3> emptyVec(WORLD_WIDTH + 1, defaultTangent);
      tangentMap.emplace_back(emptyVec);
    }
  for (unsigned int y = 1; y < map.size() - 1; y++)
    {
      for (unsigned int x = 1; x < map[0].size() - 1; x++)
        tangentMap[y][x] = glm::normalize(glm::cross(normalMap[y][x], vec3(0.0f, 1.0f, 0.0f)));
    }
}

void HillsGenerator::generateBitangentMap()
{
  using glm::vec3;
  bitangentMap.clear();
  bitangentMap.reserve(WORLD_HEIGHT + 1);
  for (size_t row = 0; row < WORLD_HEIGHT + 1; row++)
    {
      vec3 defaultBitangent(0.0f, 0.0f, 1.0f);
      std::vector<vec3> emptyVec(WORLD_WIDTH + 1, defaultBitangent);
      bitangentMap.emplace_back(emptyVec);
    }
  for (unsigned int y = 1; y < map.size() - 1; y++)
    {
      for (unsigned int x = 1; x < map[0].size() - 1; x++)
        bitangentMap[y][x] = glm::normalize(glm::cross(normalMap[y][x], tangentMap[y][x]));
    }
}

HillsGenerator::HillVertex::HillVertex(glm::vec3 pos, glm::vec2 texCoords, glm::vec3 normal, glm::vec3 tangent, glm::vec3 bitangent)
  :
    posX(pos.x - HALF_WORLD_WIDTH), posY(pos.y), posZ(pos.z - HALF_WORLD_HEIGHT),
    texCoordX(texCoords.x), texCoordY(texCoords.y),
    normalX(normal.x), normalY(normal.y), normalZ(normal.z),
    tangentX(tangent.x), tangentY(tangent.y), tangentZ(tangent.z),
    bitangentX(bitangent.x), bitangentY(bitangent.y), bitangentZ(bitangent.z)
{}
