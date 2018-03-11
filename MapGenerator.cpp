#include "MapGenerator.h"

MapGenerator::MapGenerator()
{
 initializeMap();
 tiles.reserve(NUM_TILES);
}

void MapGenerator::initializeMap()
{
  map.clear();
  map.reserve(TILES_HEIGHT + 1);
  for (size_t row = 0; row < TILES_HEIGHT + 1; row++)
    {
      std::vector<float> rowN;
      rowN.reserve(TILES_WIDTH + 1);
      map.emplace_back(rowN);
    }
  for (auto& row : map)
    for (size_t elem = 0; elem < TILES_WIDTH + 1; elem++)
      row.emplace_back(0);
}

void MapGenerator::resetAllGLBuffers()
{
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MapGenerator::setupGLBuffersAttributes()
{
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
}

void MapGenerator::createTiles(bool flat, bool createOnZeroTiles)
{
  for (unsigned int y = 1; y < map.size(); y++)
    {
      for (unsigned int x = 1; x < map[0].size(); x++)
        {
          if (map[y][x] == DENY_TILE_RENDER_VALUE)
            continue;
          bool toCreate;
          if (!flat)
            toCreate = map[y][x] != 0 || map[y-1][x] != 0 || map[y][x-1] != 0 || map[y-1][x-1] != 0;
          else
            toCreate = map[y][x] != 0;
          if (toCreate || createOnZeroTiles)
            {
              float ll;
              float lr;
              float ur;
              float ul;
              if (!flat)
                {
                  ll = map[y][x-1];
                  if (ll == DENY_TILE_RENDER_VALUE)
                    ll = map[y][x];
                  lr = map[y][x];
                  ur = map[y-1][x];
                  if (ur == DENY_TILE_RENDER_VALUE)
                    ur = map[y][x];
                  ul = map[y-1][x-1];
                  if (ul == DENY_TILE_RENDER_VALUE)
                    ul = map[y][x];
                }
              else
                {
                  ll = map[y][x];
                  lr = map[y][x];
                  ur = map[y][x];
                  ul = map[y][x];
                }
              tiles.emplace_back(x, y, ll, lr, ur, ul);
            }
        }
    }
}

std::vector<std::vector<float> > &MapGenerator::getMap()
{
  return map;
}

std::vector<TerrainTile> &MapGenerator::getTiles()
{
  return tiles;
}
