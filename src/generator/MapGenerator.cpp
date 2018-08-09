#include "MapGenerator.h"

MapGenerator::MapGenerator()
{
 initializeMap(map);
 tiles.reserve(NUM_TILES);
}

void MapGenerator::initializeMap(std::vector<std::vector<float>>& map)
{
  map.clear();
  map.reserve(TILES_HEIGHT + 1);
  for (size_t row = 0; row < TILES_HEIGHT + 1; row++)
    {
      map.emplace_back(std::vector<float>(TILES_WIDTH + 1, 0));
    }
}

void MapGenerator::resetAllGLBuffers()
{
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MapGenerator::createTiles(bool flat, bool createOnZeroTiles, std::vector<std::vector<float>>& map, float offsetY)
{
  tiles.clear();
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
                    ll = map[y][x-1] + offsetY;
                    if (ll == DENY_TILE_RENDER_VALUE)
                      ll = map[y][x];
                    lr = map[y][x] + offsetY;
                    ur = map[y-1][x] + offsetY;
                    if (ur == DENY_TILE_RENDER_VALUE)
                      ur = map[y][x];
                    ul = map[y-1][x-1] + offsetY;
                    if (ul == DENY_TILE_RENDER_VALUE)
                      ul = map[y][x];
                  }
                else
                  {
                    ll = lr = ur = ul = map[y][x];
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

GLuint &MapGenerator::getVAO()
{
  return vao;
}

GLuint &MapGenerator::getVBO()
{
  return vbo;
}

GLuint &MapGenerator::getEBO()
{
  return ebo;
}

void MapGenerator::deleteGLObjects()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
}
