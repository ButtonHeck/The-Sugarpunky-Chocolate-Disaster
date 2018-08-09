#include "generator/HillsMapGenerator.h"

HillsMapGenerator::HillsMapGenerator(std::vector<std::vector<float> > &waterMap)
  :
    MapGenerator(),
    waterMap(waterMap)
{
  randomizer.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

HillsMapGenerator::~HillsMapGenerator()
{
  deleteGLObjects();
}

void HillsMapGenerator::prepareMap()
{
  float max_height = 0.0f;
  generateMap(12, &max_height, HILL_DENSITY::DENSE);
  generateMap(6, &max_height, HILL_DENSITY::THIN);
  compressMap(0.02f, &max_height, 1.25f); //compress entire range
  compressMap(0.66f, &max_height, 2.2f); //compress top-most peaks
  removeMapPlateaus(1.0f);
  for (unsigned int i = 0; i < 4; i++)
    {
      smoothMapHeightChunks(0.6f, 0.05f, 0.05f);
    }
  removeOrphanHills();
  smoothMapSinks();
  createTiles();
  tiles.shrink_to_fit();
}

void HillsMapGenerator::fillBufferData(bool textureSlopeCorrection)
{
  const size_t VERTEX_DATA_LENGTH = tiles.size() * 48;
  const size_t ELEMENT_DATA_LENGTH = tiles.size() * 6;
  GLfloat* vertices = new GLfloat[VERTEX_DATA_LENGTH];
  GLuint* indices = new GLuint[ELEMENT_DATA_LENGTH];
  glm::vec3 normal1, normal2;
  for (unsigned int c = 0; c < tiles.size(); c++)
    {
      TerrainTile& tile = tiles[c];
      int offset = c * 48;
      int index = c * 6;
      //approximation for texture mapping based on height coords of the tile.
      //for now, it works only for tiles which have a slope for either left->right and top->bottom (or vice versa) direction
      //generally speaking it doesn't work for tiles with one of the following principal scheme:
      /*
       * UL UR      0 1       1 0       0 0       0 0
       * LL LR  ->  0 0   or  0 0   or  1 0   or  0 1
       */
      float dyRatio = 1.0, dxRatio = 1.0;
      if (textureSlopeCorrection)
        {
          if ((tile.upperLeft > tile.lowLeft && tile.upperRight > tile.lowRight && tile.upperLeft > tile.lowRight && tile.upperRight > tile.lowLeft)
              || (tile.upperLeft < tile.lowLeft && tile.upperRight < tile.lowRight && tile.upperLeft < tile.lowRight && tile.upperRight < tile.lowLeft))
            {
              dyRatio = std::max(std::abs((tile.upperLeft + tile.upperRight) / 2 - (tile.lowLeft + tile.lowRight) / 2), 1.0f);
            }
          if ((tile.upperLeft > tile.upperRight && tile.upperLeft > tile.lowRight && tile.lowLeft > tile.upperRight && tile.lowLeft > tile.lowRight)
              || (tile.upperLeft < tile.upperRight && tile.upperLeft < tile.lowRight && tile.lowLeft < tile.upperRight && tile.lowLeft < tile.lowRight))
            {
              dxRatio = std::max(std::abs((tile.lowLeft + tile.upperLeft) / 2 - (tile.lowRight + tile.upperRight) / 2), 1.0f);
            }
        }
      bool indicesCrossed = false;
      if (tile.lowRight < tile.upperLeft || tile.upperLeft < tile.lowRight)
        indicesCrossed = true;

      float texCoordXOffset = (tile.mapX % 2) / 2.0f;
      float texCoordYOffset = ((TILES_HEIGHT - tile.mapY) % 2) / 2.0f;

      if (!indicesCrossed)
        {
          normal1 = glm::vec3(tile.lowLeft - tile.lowRight, 1, tile.upperRight - tile.lowRight);
          normal2 = glm::vec3(tile.upperLeft - tile.upperRight, 1, tile.upperLeft - tile.lowLeft);
          //ll1
          vertices[offset] =   -1- HALF_TILES_WIDTH + tile.mapX;
          vertices[offset+1] = tile.lowLeft;
          vertices[offset+2] = - HALF_TILES_HEIGHT + tile.mapY;
          vertices[offset+3] = 0.0f + texCoordXOffset;
          vertices[offset+4] = 0.0f + texCoordYOffset;
          vertices[offset+5] = normal1.x;
          vertices[offset+6] = normal1.y;
          vertices[offset+7] = normal1.z;
          //lr1
          vertices[offset+8] =  - HALF_TILES_WIDTH + tile.mapX;
          vertices[offset+9] =  tile.lowRight;
          vertices[offset+10] = - HALF_TILES_HEIGHT + tile.mapY;
          vertices[offset+11] = (1.0f / 2.0f + texCoordXOffset) * dxRatio;
          vertices[offset+12] = 0.0f + texCoordYOffset;
          vertices[offset+13] = normal1.x;
          vertices[offset+14] = normal1.y;
          vertices[offset+15] = normal1.z;
          //ur1
          vertices[offset+16] = - HALF_TILES_WIDTH + tile.mapX;
          vertices[offset+17] = tile.upperRight;
          vertices[offset+18] = -1 - HALF_TILES_HEIGHT + tile.mapY;
          vertices[offset+19] = (1.0f / 2.0f + texCoordXOffset) * dxRatio;
          vertices[offset+20] = (1.0f / 2.0f + texCoordYOffset) * dyRatio;
          vertices[offset+21] = normal1.x;
          vertices[offset+22] = normal1.y;
          vertices[offset+23] = normal1.z;
          //ur2
          vertices[offset+24] = - HALF_TILES_WIDTH + tile.mapX;
          vertices[offset+25] = tile.upperRight;
          vertices[offset+26] = -1 - HALF_TILES_HEIGHT + tile.mapY;
          vertices[offset+27] = (1.0f / 2.0f + texCoordXOffset) * dxRatio;
          vertices[offset+28] = (1.0f / 2.0f + texCoordYOffset) * dyRatio;
          vertices[offset+29] = normal2.x;
          vertices[offset+30] = normal2.y;
          vertices[offset+31] = normal2.z;
          //ul2
          vertices[offset+32] = -1 - HALF_TILES_WIDTH + tile.mapX;
          vertices[offset+33] = tile.upperLeft;
          vertices[offset+34] = -1 - HALF_TILES_HEIGHT + tile.mapY;
          vertices[offset+35] = 0.0f + texCoordXOffset;
          vertices[offset+36] = (1.0f / 2.0f + texCoordYOffset) * dyRatio;
          vertices[offset+37] = normal2.x;
          vertices[offset+38] = normal2.y;
          vertices[offset+39] = normal2.z;
          //ll2
          vertices[offset+40] = -1- HALF_TILES_WIDTH + tile.mapX;
          vertices[offset+41] = tile.lowLeft;
          vertices[offset+42] = - HALF_TILES_HEIGHT + tile.mapY;
          vertices[offset+43] = 0.0f + texCoordXOffset;
          vertices[offset+44] = 0.0f + texCoordYOffset;
          vertices[offset+45] = normal2.x;
          vertices[offset+46] = normal2.y;
          vertices[offset+47] = normal2.z;
        }
      else
        {
          normal1 = glm::vec3(tile.lowLeft - tile.lowRight, 1, tile.upperLeft - tile.lowLeft);
          normal2 = glm::vec3(tile.upperLeft - tile.upperRight, 1, tile.upperRight - tile.lowRight);
          //ul1
          vertices[offset] =   -1 - HALF_TILES_WIDTH + tile.mapX;
          vertices[offset+1] = tile.upperLeft;
          vertices[offset+2] = -1 - HALF_TILES_HEIGHT + tile.mapY;
          vertices[offset+3] = 0.0f + texCoordXOffset;
          vertices[offset+4] = (1.0f / 2.0f + texCoordYOffset) * dyRatio;
          vertices[offset+5] = normal1.x;
          vertices[offset+6] = normal1.y;
          vertices[offset+7] = normal1.z;
          //ll1
          vertices[offset+8] =  -1- HALF_TILES_WIDTH + tile.mapX;
          vertices[offset+9] =  tile.lowLeft;
          vertices[offset+10] = - HALF_TILES_HEIGHT + tile.mapY;
          vertices[offset+11] = 0.0f + texCoordXOffset;
          vertices[offset+12] = 0.0f + texCoordYOffset;
          vertices[offset+13] = normal1.x;
          vertices[offset+14] = normal1.y;
          vertices[offset+15] = normal1.z;
          //lr1
          vertices[offset+16] = - HALF_TILES_WIDTH + tile.mapX;
          vertices[offset+17] = tile.lowRight;
          vertices[offset+18] = - HALF_TILES_HEIGHT + tile.mapY;
          vertices[offset+19] = (1.0f / 2.0f + texCoordXOffset) * dxRatio;
          vertices[offset+20] = 0.0f + texCoordYOffset;
          vertices[offset+21] = normal1.x;
          vertices[offset+22] = normal1.y;
          vertices[offset+23] = normal1.z;
          //lr2
          vertices[offset+24] = - HALF_TILES_WIDTH + tile.mapX;
          vertices[offset+25] = tile.lowRight;
          vertices[offset+26] = - HALF_TILES_HEIGHT + tile.mapY;
          vertices[offset+27] = (1.0f / 2.0f + texCoordXOffset) * dxRatio;
          vertices[offset+28] = 0.0f + texCoordYOffset;
          vertices[offset+29] = normal2.x;
          vertices[offset+30] = normal2.y;
          vertices[offset+31] = normal2.z;
          //ur2
          vertices[offset+32] = - HALF_TILES_WIDTH + tile.mapX;
          vertices[offset+33] = tile.upperRight;
          vertices[offset+34] = -1 - HALF_TILES_HEIGHT + tile.mapY;
          vertices[offset+35] = (1.0f / 2.0f + texCoordXOffset) * dxRatio;
          vertices[offset+36] = (1.0f / 2.0f + texCoordYOffset) * dyRatio;
          vertices[offset+37] = normal2.x;
          vertices[offset+38] = normal2.y;
          vertices[offset+39] = normal2.z;
          //ul2
          vertices[offset+40] = -1 - HALF_TILES_WIDTH + tile.mapX;
          vertices[offset+41] = tile.upperLeft;
          vertices[offset+42] = -1 - HALF_TILES_HEIGHT + tile.mapY;
          vertices[offset+43] = 0.0f + texCoordXOffset;
          vertices[offset+44] = (1.0f / 2.0f + texCoordYOffset) * dyRatio;
          vertices[offset+45] = normal2.x;
          vertices[offset+46] = normal2.y;
          vertices[offset+47] = normal2.z;
        }
      indices[index] = index;
      indices[index+1] = index + 1;
      indices[index+2] = index + 2;
      indices[index+3] = index + 3;
      indices[index+4] = index + 4;
      indices[index+5] = index + 5;
    }
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * ELEMENT_DATA_LENGTH, indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * VERTEX_DATA_LENGTH, vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
  resetAllGLBuffers();
  delete[] vertices;
  delete[] indices;
}

void HillsMapGenerator::createTiles()
{
  tiles.clear();
  for (unsigned int y = 1; y < map.size(); y++)
    {
      for (unsigned int x = 1; x < map[0].size(); x++)
        {
          if (map[y][x] == DENY_TILE_RENDER_VALUE)
            continue;
          bool toCreate = map[y][x] != 0 || map[y-1][x] != 0 || map[y][x-1] != 0 || map[y-1][x-1] != 0;
            if (toCreate)
              {
                float ll = map[y][x-1] + HILLS_OFFSET_Y;
                float lr = map[y][x] + HILLS_OFFSET_Y;
                float ur = map[y-1][x] + HILLS_OFFSET_Y;
                float ul = map[y-1][x-1] + HILLS_OFFSET_Y;
                tiles.emplace_back(x, y, ll, lr, ur, ul);
              }
        }
    }
}

void HillsMapGenerator::generateMap(int cycles, float *max_height, HILL_DENSITY density)
{
  std::uniform_real_distribution<float> heightDistribution(0.3f, 0.8f);
  float density_value = 3.0f * (float)TILES_WIDTH;
  if (density == HILL_DENSITY::THIN)
      density_value = 3.1f * (float)TILES_WIDTH;
  else if (density == HILL_DENSITY::DENSE)
    density_value = 2.9f * (float)TILES_WIDTH;

  //hills kernel generation cycle
  for (int y = 1; y < TILES_HEIGHT - 1; y++)
    {
      for (int x = 1; x < TILES_WIDTH - 1; x++)
        {
          if (rand() % (int)density_value == 0 && !hasWaterNearby(x, y, cycles - 1))
            {
              map[y][x] += 1.0f;
            }
        }
    }
  *max_height = 1.0f;

  //fattening hills around, based on their kernel positions
  for (int cycle = 1; cycle < cycles; cycle++)
    {
      for (int y = cycle; y < TILES_HEIGHT - cycle; y++)
        {
          for (int x = cycle; x < TILES_WIDTH - cycle; x++)
            {
              if (y >= TILES_HEIGHT)
                break;
              if (rand() % (cycle+1) == cycle && (map[y][x] != 0))
                {
                  int left = x-cycle;
                  if (left <= cycle)
                    left = cycle;
                  int right = x+cycle;
                  if (right >= TILES_WIDTH - cycle - 1)
                    right = TILES_WIDTH - cycle - 1;
                  int top = y-cycle;
                  if (top <= cycle)
                    top = cycle;
                  int bottom = y+cycle;
                  if (bottom >= TILES_HEIGHT - cycle - 1)
                    bottom = TILES_HEIGHT - cycle - 1;
                  for (int y2 = top; y2 <= bottom; y2++)
                    {
                      for (int x2 = left; x2 <= right; x2++)
                        {
                          if (rand() % (cycle + 2) > 1)
                            {
                              if (hasWaterNearby(x2, y2, 4))
                                {
                                  --bottom;
                                  --right;
                                  continue;
                                }
                              map[y2][x2] += 1.0f - 0.05f * cycle + (heightDistribution(randomizer));
                              if (map[y2][x2] > *max_height)
                                *max_height = map[y2][x2];
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

bool HillsMapGenerator::hasWaterNearby(unsigned int x, unsigned int y, unsigned int radius)
{
  int xl = x - radius;
  if (xl <= 0)
    xl = 0;
  int xr = x + radius;
  if (xr >= TILES_WIDTH)
    xr = TILES_WIDTH;
  int yt = y - radius;
  if (yt <= 0)
    yt = 0;
  int yb = y + radius;
  if (yb >= TILES_HEIGHT)
    yb = TILES_HEIGHT;
  for (int x1 = xl; x1 <= xr; x1++)
    {
      for (int y1 = yt; y1 <= yb; y1++)
        {
          if (waterMap[y1][x1] != 0)
            {
              return true;
            }
        }
    }
  return false;
}

void HillsMapGenerator::compressMap(float threshold_percent, float *limit, float ratio)
{
  float threshold_value = *limit * threshold_percent;
  for (auto& row : map)
    for (auto& height : row)
      {
        if (height < threshold_value)
          continue;
        height = threshold_value + (height - threshold_value) / ratio;
      }
  *limit /= ratio;
}

void HillsMapGenerator::removeMapPlateaus(float plateauHeight)
{
  unsigned int yt, yb, xl, xr;
  for (unsigned int y = 1; y < TILES_HEIGHT - 1; y++)
    {
      for (unsigned int x = 1; x < TILES_WIDTH - 1; x++)
        {
          if (map[y][x] == 0)
            continue;
          unsigned int plateauHeightNeighbourTiles = 0, neighboursLimit = 6;
          yt = y - 1;
          yb = y + 1;
          xl = x - 1;
          xr = x + 1;
          for (auto y1 = yt; y1 <= yb; y1++)
            {
              for (auto x1 = xl; x1 <= xr; x1++)
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

void HillsMapGenerator::smoothMapHeightChunks(float baseWeight, float evenWeight, float diagonalWeight)
{
  std::vector<std::vector<float>> hillMapSmoothed;
  initializeMap(hillMapSmoothed);
  for (unsigned int y = 1; y < TILES_HEIGHT - 1; y++)
    {
      for (unsigned int x = 1; x < TILES_WIDTH - 1; x++)
        {
          if (map[y][x] == 0)
            continue;
          float smoothedHeight =
                map[y][x] * baseWeight
              + map[y-1][x] * evenWeight
              + map[y+1][x] * evenWeight
              + map[y][x-1] * evenWeight
              + map[y][x+1] * evenWeight
              + map[y-1][x-1] * diagonalWeight
              + map[y-1][x+1] * diagonalWeight
              + map[y+1][x-1] * diagonalWeight
              + map[y+1][x+1] * diagonalWeight;
          hillMapSmoothed[y][x] = smoothedHeight;
        }
    }
  map.assign(hillMapSmoothed.begin(), hillMapSmoothed.end());
}

void HillsMapGenerator::removeOrphanHills()
{
  for (int y = 1; y < TILES_HEIGHT; y++)
    {
      for (int x = 1; x < TILES_WIDTH; x++)
        {
          if (map[y][x] != 0.0f && isOrphanAt(x, y))
            map[y][x] = 0.0f;
        }
    }
}

bool HillsMapGenerator::isOrphanAt(int x, int y)
{
  return (map[y-1][x-1] == 0 &&
          map[y-1][x] == 0 &&
          map[y-1][x+1] == 0 &&
          map[y][x-1] == 0 &&
          map[y][x+1] == 0 &&
          map[y+1][x-1] == 0 &&
          map[y+1][x] == 0 &&
          map[y+1][x+1] == 0);
}

void HillsMapGenerator::smoothMapSinks()
{
  for (unsigned int y = 1; y < TILES_HEIGHT; y++)
    {
      for (unsigned int x = 1; x < TILES_WIDTH; x++)
        {
          unsigned int higherNeighbours = 0;
          if (map[y][x] < map[y-1][x-1])
            ++higherNeighbours;
          if (map[y][x] < map[y-1][x])
            ++higherNeighbours;
          if (map[y][x] < map[y-1][x+1])
            ++higherNeighbours;
          if (map[y][x] < map[y][x-1])
            ++higherNeighbours;
          if (map[y][x] < map[y][x+1])
            ++higherNeighbours;
          if (map[y][x] < map[y+1][x-1])
            ++higherNeighbours;
          if (map[y][x] < map[y+1][x])
            ++higherNeighbours;
          if (map[y][x] < map[y+1][x+1])
            ++higherNeighbours;
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
