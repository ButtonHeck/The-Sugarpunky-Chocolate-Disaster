#include "HillsMapGenerator.h"

HillsMapGenerator::HillsMapGenerator(std::vector<std::vector<float> > &waterMap)
  :
    MapGenerator(),
    waterMap(waterMap)
{}

void HillsMapGenerator::prepareMap()
{
  float max_height = 0.0f;
  generateHillMap(8, &max_height, HILL_DENSITY::DENSE);
  generateHillMap(4, &max_height, HILL_DENSITY::THIN);
  compressHeightHillMap(0.15f, &max_height, 1.5f); //slightly compress entire height range
  compressHeightHillMap(0.66f, &max_height, 5.0f); //more heavy compress for top-most peaks
  removeHillMapPlateaus(1.0f);
  smoothHillMapHeightChunks(0.7f, 0.05f, 0.025f);
  removeOrphanHills();
  smoothHillMapSinks();
  createTiles(false, false);
  tiles.shrink_to_fit();
}

void HillsMapGenerator::fillHillsBuffersData()
{
  const size_t VERTEX_DATA_LENGTH = tiles.size() * 20;
  const size_t ELEMENT_DATA_LENGTH = tiles.size() * 6;
  GLfloat vertices[VERTEX_DATA_LENGTH];
  GLuint indices[ELEMENT_DATA_LENGTH];
  for (unsigned int i = 0; i < tiles.size(); i++)
    {
      TerrainTile& tile = tiles[i];
      int offset = i * 20;
      int indexArrayOffset = i * 6;
      int index = i * 4;
      //approximation for texture mapping based on height coords of the tile.
      //for now, it works only for tiles which have a slope for either left->right and top->bottom (or vice versa) direction
      //generally speaking it doesn't work for tiles with one of the following principal scheme:
      /*
       * UL UR      0 1       1 0       0 0       0 0
       * LL LR  ->  0 0   or  0 0   or  1 0   or  0 1
       */
      float dyRatio = 1.0, dxRatio = 1.0;
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
      //ll
      vertices[offset] = -1- TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+1] = tile.lowLeft;
      vertices[offset+2] = - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+3] = 0.0f;
      vertices[offset+4] = 0.0f;
      //lr
      vertices[offset+5] = - TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+6] = tile.lowRight;
      vertices[offset+7] = - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+8] = 1.0f * dxRatio;
      vertices[offset+9] = 0.0f;
      //ur
      vertices[offset+10] = - TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+11] = tile.upperRight;
      vertices[offset+12] = -1 - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+13] = 1.0f * dxRatio;
      vertices[offset+14] = 1.0f * dyRatio;
      //ul
      vertices[offset+15] = -1 - TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+16] = tile.upperLeft;
      vertices[offset+17] = -1 - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+18] = 0.0f;
      vertices[offset+19] = 1.0f * dyRatio;

      bool indicesCrossed = false;
      if (tile.lowRight < tile.upperLeft || tile.upperLeft < tile.lowRight)
        indicesCrossed = true;

      if (!indicesCrossed)
        {
          indices[indexArrayOffset] = index;
          indices[indexArrayOffset+1] = index + 1;
          indices[indexArrayOffset+2] = index + 2;
          indices[indexArrayOffset+3] = index + 2;
          indices[indexArrayOffset+4] = index + 3;
          indices[indexArrayOffset+5] = index;
        }
      else
        {
          indices[indexArrayOffset] = index + 1;
          indices[indexArrayOffset+1] = index + 2;
          indices[indexArrayOffset+2] = index + 3;
          indices[indexArrayOffset+3] = index + 3;
          indices[indexArrayOffset+4] = index;
          indices[indexArrayOffset+5] = index + 1;
        }
    }
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  setupGLBuffersAttributes();
  resetAllGLBuffers();
}

void HillsMapGenerator::generateHillMap(int cycles, float *max_height, HILL_DENSITY density)
{
  srand(time(NULL));
  std::uniform_real_distribution<float> heightDistribution(0.3f, 0.7f);
  std::default_random_engine randomizer;
  float density_value = 1.0f * (float)TILES_WIDTH;
  if (density == HILL_DENSITY::THIN)
      density_value = 1.1f * (float)TILES_WIDTH;
  else if (density == HILL_DENSITY::DENSE)
    density_value = 0.9f * (float)TILES_WIDTH;

  //hills kernel generation cycle
  for (int y = 1; y < TILES_HEIGHT - 1; y++)
    {
      for (int x = 1; x < TILES_WIDTH - 1; x++)
        {
          if (rand() % (int)density_value == 0 && !hasWaterNearby(x, y, 2))
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
                          if (rand() % (cycle + 2) > 1 && !hasWaterNearby(x2, y2, 2))
                            {
                              map[y2][x2] += 1.0f - 0.075f * cycle + (heightDistribution(randomizer) / cycle);
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

void HillsMapGenerator::compressHeightHillMap(float threshold_percent, float *limit, float ratio)
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

void HillsMapGenerator::removeHillMapPlateaus(float plateauHeight)
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

void HillsMapGenerator::smoothHillMapHeightChunks(float baseWeight, float evenWeight, float diagonalWeight)
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

void HillsMapGenerator::smoothHillMapSinks()
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

void HillsMapGenerator::draw(Shader &shader)
{
  shader.setInt("surfaceTextureEnum", 2);
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 6 * tiles.size(), GL_UNSIGNED_INT, 0);
}
