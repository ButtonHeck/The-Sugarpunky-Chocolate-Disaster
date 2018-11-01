#include "game/world/terrain/shore/ShoreGenerator.h"

ShoreGenerator::ShoreGenerator(const map2D_f &waterMap)
  :
    Generator(),
    waterMap(waterMap)
{
  randomizer.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

void ShoreGenerator::setup()
{
  generateMap();
  for (unsigned int i = 0; i < 5; i++)
    {
      smoothMap();
      float selfWeight = 0.5f - 0.02f * i;
      float evenWeight = (1.0f - selfWeight) / 8.0f;
      float diagonalWeight = evenWeight;
      smoothMapHeightChunks(map, selfWeight, evenWeight, diagonalWeight);
    }
  randomizeShore();
  compressMap(2.0f);
  correctMapAtEdges();
  removeUnderwaterTiles(SHORE_CLIP_LEVEL);
  createTiles();
  tiles.shrink_to_fit();
  smoothNormals(map, normalMap);
  fillBufferData();
}

void ShoreGenerator::generateMap()
{
  std::uniform_real_distribution<float> distribution(0.9f, 1.1f);
  for (unsigned int y = 0; y <= WORLD_HEIGHT; y++)
    {
      for (unsigned int x = 0; x <= WORLD_WIDTH; x++)
        map[y][x] = waterMap[y][x] * 1.1f * distribution(randomizer);
    }
}

void ShoreGenerator::smoothMap()
{
  float waterLevel = WATER_LEVEL + 0.25f;
  //smooth tile below on map
  for (unsigned int y = 1; y < WORLD_HEIGHT + 1; y++)
    {
      for (unsigned int x = 0; x < WORLD_WIDTH + 1; x++)
        {
          if (map[y-1][x] < waterLevel - waterLevel * 0.25f)
            map[y][x] += waterLevel * 0.5f;
        }
    }
  //smooth tile upper on map
  for (unsigned int y = 0; y < WORLD_HEIGHT; y++)
    {
      for (unsigned int x = 0; x < WORLD_WIDTH + 1; x++)
        {
          if (map[y+1][x] < waterLevel - waterLevel * 0.25f)
            map[y][x] += waterLevel * 0.5f;
        }
    }
  //smooth tile left on map
  for (unsigned int y = 0; y < WORLD_HEIGHT + 1; y++)
    {
      for (unsigned int x = 0; x < WORLD_WIDTH; x++)
        {
          if (map[y][x+1] < waterLevel - waterLevel * 0.25f)
            map[y][x] += waterLevel * 0.5f;
        }
    }
  //smooth tile right on map
  for (unsigned int y = 0; y < WORLD_HEIGHT + 1; y++)
    {
      for (unsigned int x = 1; x < WORLD_WIDTH + 1; x++)
        {
          if (map[y][x-1] < waterLevel - waterLevel * 0.25f)
            map[y][x] += waterLevel * 0.5f;
        }
    }
}

void ShoreGenerator::randomizeShore()
{
  std::uniform_real_distribution<float> distribution(-0.24f, 0.24f);
  for (unsigned int y = 0; y < WORLD_HEIGHT; y++)
    {
      for (unsigned int x = 0; x < WORLD_WIDTH; x++)
        {
          if (map[y][x] < 0)
            map[y][x] += distribution(randomizer);
        }
    }
}

void ShoreGenerator::correctMapAtEdges()
{
  //correct top and bottom sides of the map
  for (unsigned int x = 0; x < WORLD_WIDTH; ++x)
    {
      if (waterMap[0][x] != 0)
        map[0][x] = map[1][x];
      if (waterMap[WORLD_HEIGHT][x] != 0)
        map[WORLD_HEIGHT][x] = map[WORLD_HEIGHT - 1][x];
    }
  //correct left and right sides of the map
  for (unsigned int y = 0; y < WORLD_HEIGHT; ++y)
    {
      if (waterMap[y][0] != 0)
        map[y][0] = map[y][1];
      if (waterMap[y][WORLD_WIDTH] != 0)
        map[y][WORLD_WIDTH] = map[y][WORLD_WIDTH - 1];
    }
}

void ShoreGenerator::compressMap(float ratio)
{
  for (std::vector<float>& row : map)
    {
      for (float& height : row)
        {
          if (height < 0)
            height /= ratio;
        }
    }
}

void ShoreGenerator::removeUnderwaterTiles(float thresholdValue)
{
  for (unsigned int y = 1; y < WORLD_HEIGHT; y++)
    {
      for (unsigned int x = 1; x < WORLD_WIDTH; x++)
        {
          if (map[y-1][x-1] < thresholdValue
              && map[y-1][x] < thresholdValue
              && map[y-1][x+1] < thresholdValue
              && map[y][x-1] < thresholdValue
              && map[y][x] < thresholdValue
              && map[y][x+1] < thresholdValue
              && map[y+1][x-1] < thresholdValue
              && map[y+1][x] < thresholdValue
              && map[y+1][x+1] < thresholdValue)
            map[y][x] = TILE_NO_RENDER_VALUE;
        }
    }
}

void ShoreGenerator::createTiles()
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
              float lowLeft = (map[y][x-1] == TILE_NO_RENDER_VALUE ? map[y][x] : map[y][x-1]);
              float lowRight = map[y][x];
              float upRight = (map[y-1][x] == TILE_NO_RENDER_VALUE ? map[y][x] : map[y-1][x]);
              float upLeft = (map[y-1][x-1] == TILE_NO_RENDER_VALUE ? map[y][x] : map[y-1][x-1]);
              tiles.emplace_back(x, y, lowLeft, lowRight, upRight, upLeft);
            }
        }
    }
}

void ShoreGenerator::fillBufferData()
{
  using glm::vec2;
  using glm::vec3;
  const size_t VERTEX_DATA_LENGTH = tiles.size() * 48;
  std::unique_ptr<GLfloat[]> vertices(new GLfloat[NUM_TILES * 48]);
  for (unsigned int i = 0; i < tiles.size(); i++)
    {
      TerrainTile& tile = tiles[i];
      int offset = i * 48;
      int x = tile.mapX, y = tile.mapY;

      ShoreVertex lowLeft(vec3(tile.mapX - 1, tile.lowLeft, tile.mapY), vec2(0.0f), normalMap[y][x-1]);
      ShoreVertex lowRight(vec3(tile.mapX, tile.lowRight, tile.mapY), vec2(1.0f, 0.0f), normalMap[y][x]);
      ShoreVertex upRight(vec3(tile.mapX, tile.upperRight, tile.mapY - 1), vec2(1.0f), normalMap[y-1][x]);
      ShoreVertex upLeft(vec3(tile.mapX - 1, tile.upperLeft, tile.mapY - 1), vec2(0.0f, 1.0f), normalMap[y-1][x-1]);

      bufferVertex(vertices.get(), offset, lowLeft); //ll1
      bufferVertex(vertices.get(), offset+8, lowRight); //lr1
      bufferVertex(vertices.get(), offset+16, upRight); //ur1
      bufferVertex(vertices.get(), offset+24, upRight); //ur2
      bufferVertex(vertices.get(), offset+32, upLeft);//ul2
      bufferVertex(vertices.get(), offset+40, lowLeft); //ll2
    }

  basicGLBuffers.bind(VAO | VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * VERTEX_DATA_LENGTH, vertices.get(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
  resetAllGLBuffers();
}

void ShoreGenerator::bufferVertex(GLfloat *vertices, int offset, ShoreGenerator::ShoreVertex vertex)
{
  vertices[offset+0] = vertex.posX;
  vertices[offset+1] = vertex.posY;
  vertices[offset+2] = vertex.posZ;
  vertices[offset+3] = vertex.texCoordX;
  vertices[offset+4] = vertex.texCoordY;
  vertices[offset+5] = vertex.normalX;
  vertices[offset+6] = vertex.normalY;
  vertices[offset+7] = vertex.normalZ;
}

ShoreGenerator::ShoreVertex::ShoreVertex(glm::vec3 position, glm::vec2 texCoords, glm::vec3 normal)
  :
    posX(position.x - HALF_WORLD_WIDTH), posY(position.y), posZ(position.z - HALF_WORLD_HEIGHT),
    texCoordX(texCoords.x), texCoordY(texCoords.y),
    normalX(normal.x), normalY(normal.y), normalZ(normal.z)
{}
