#include "MapGenerator.h"

MapGenerator::MapGenerator()
{
 initializeMap(map);
 tiles.reserve(NUM_TILES);
 glCreateVertexArrays(1, &vao);
 glCreateBuffers(1, &vbo);
 glCreateBuffers(1, &ebo);
}

void MapGenerator::initializeMap(std::vector<std::vector<float>>& map)
{
  map.clear();
  map.reserve(WORLD_HEIGHT + 1);
  for (size_t row = 0; row < WORLD_HEIGHT + 1; row++)
    {
      map.emplace_back(std::vector<float>(WORLD_WIDTH + 1, 0));
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
          if (map[y][x] == TILE_NO_RENDER_VALUE)
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
                    if (ll == TILE_NO_RENDER_VALUE)
                      ll = map[y][x];
                    lr = map[y][x] + offsetY;
                    ur = map[y-1][x] + offsetY;
                    if (ur == TILE_NO_RENDER_VALUE)
                      ur = map[y][x];
                    ul = map[y-1][x-1] + offsetY;
                    if (ul == TILE_NO_RENDER_VALUE)
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

GLuint MapGenerator::getVAO() const
{
  return vao;
}

GLuint MapGenerator::getVBO() const
{
  return vbo;
}

GLuint MapGenerator::getEBO() const
{
  return ebo;
}

void MapGenerator::deleteGLObjects()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
}

void MapGenerator::serialize(std::ofstream &output)
{
  for (std::vector<float>& row : map)
    {
      for (float& value : row)
        {
          output << value << " ";
        }
    }
}

void MapGenerator::deserialize(std::ifstream &input)
{
  for (std::vector<float>& row : map)
    {
      for (float& value : row)
        {
          input >> value;
        }
    }
}
