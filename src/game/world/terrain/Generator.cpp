#include "game/world/terrain/Generator.h"

Generator::Generator()
  :
    basicGLBuffers(VAO | VBO | EBO)
{
 initializeMap(map);
 tiles.reserve(NUM_TILES);
}

void Generator::resetAllGLBuffers()
{
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Generator::createTiles(bool flat, bool createOnZeroTiles, map2D_f& map, float offsetY)
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
                float lowLeft;
                float lowRight;
                float upRight;
                float upLeft;
                if (!flat)
                  {
                    lowLeft = (map[y][x-1] + offsetY == TILE_NO_RENDER_VALUE ? map[y][x] : map[y][x-1] + offsetY);
                    lowRight = map[y][x] + offsetY;
                    upRight = (map[y-1][x] + offsetY == TILE_NO_RENDER_VALUE ? map[y][x] : map[y-1][x] + offsetY);
                    upLeft = (map[y-1][x-1] + offsetY == TILE_NO_RENDER_VALUE ? map[y][x] : map[y-1][x-1] + offsetY);
                  }
                else
                  lowLeft = lowRight = upRight = upLeft = map[y][x];
                tiles.emplace_back(x, y, lowLeft, lowRight, upRight, upLeft);
              }
        }
    }
}

const map2D_f &Generator::getMap() const
{
  return map;
}

std::vector<TerrainTile> &Generator::getTiles()
{
  return tiles;
}

void Generator::serialize(std::ofstream &output)
{
  for (std::vector<float>& row : map)
    {
      for (float& value : row)
        output << value << " ";
    }
}

void Generator::deserialize(std::ifstream &input)
{
  for (std::vector<float>& row : map)
    {
      for (float& value : row)
        input >> value;
    }
}

void Generator::smoothMapHeightChunks(map2D_f &map, float selfWeight, float evenWeight, float diagonalWeight)
{
  map2D_f shoreMapSmoothed;
  Generator::initializeMap(shoreMapSmoothed);
  for (unsigned int y = 1; y < WORLD_HEIGHT; y++)
    {
      for (unsigned int x = 1; x < WORLD_WIDTH; x++)
        {
          if (map[y][x] == 0)
            continue;
          float smoothedHeight =
                map[y][x] * selfWeight
              + map[y-1][x] * evenWeight
              + map[y+1][x] * evenWeight
              + map[y][x-1] * evenWeight
              + map[y][x+1] * evenWeight
              + map[y-1][x-1] * diagonalWeight
              + map[y-1][x+1] * diagonalWeight
              + map[y+1][x-1] * diagonalWeight
              + map[y+1][x+1] * diagonalWeight;
          shoreMapSmoothed[y][x] = smoothedHeight;
        }
    }
  map.assign(shoreMapSmoothed.begin(), shoreMapSmoothed.end());
}

void Generator::smoothNormals(map2D_f &map, map2D_vec3 &normalMap)
{
  using glm::vec3;
  normalMap.clear();
  normalMap.reserve(WORLD_HEIGHT + 1);
  for (size_t row = 0; row < WORLD_HEIGHT + 1; row++)
    {
      vec3 defaultNormal(0.0f, 1.0f, 0.0f);
      std::vector<vec3> emptyVec(WORLD_WIDTH + 1, defaultNormal);
      normalMap.emplace_back(emptyVec);
    }
  for (unsigned int y = 1; y < map.size() - 1; y++)
    {
      for (unsigned int x = 1; x < map[0].size() - 1; x++)
        {
          vec3 n0 = glm::normalize(vec3(map[y][x-1] - map[y][x], 1, map[y-1][x] - map[y][x]));
          vec3 n3 = glm::normalize(vec3(map[y][x] - map[y][x+1], 1, map[y-1][x+1] - map[y][x+1]));
          vec3 n6 = glm::normalize(vec3(map[y+1][x-1] - map[y+1][x], 1, map[y][x] - map[y+1][x]));
          vec3 n1 = glm::normalize(vec3(map[y-1][x] - map[y-1][x+1], 1, map[y-1][x] - map[y][x]));
          vec3 n4 = glm::normalize(vec3(map[y][x] - map[y][x+1], 1, map[y][x] - map[y+1][x]));
          vec3 n9 = glm::normalize(vec3(map[y][x-1] - map[y][x], 1, map[y][x-1] - map[y+1][x-1]));
          vec3 avgNormal = glm::normalize(n0 + n1 + n3 + n4 + n6 + n9);
          normalMap[y][x] = avgNormal;
        }
    }
}
