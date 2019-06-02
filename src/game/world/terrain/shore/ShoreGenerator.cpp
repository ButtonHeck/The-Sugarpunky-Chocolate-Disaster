#include "ShoreGenerator.h"

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
  for (unsigned int cycleCount = 0; cycleCount < SHORE_SMOOTH_CYCLES; cycleCount++)
    {
      smoothMap();
      float selfWeight = 0.5f - 0.02f * cycleCount;
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
  std::uniform_real_distribution<float> positionDistribution(MIN_HEIGHT_KERNEL_OFFSET, MAX_HEIGHT_KERNEL_OFFSET);
  for (unsigned int y = 0; y <= WORLD_HEIGHT; y++)
    {
      for (unsigned int x = 0; x <= WORLD_WIDTH; x++)
        map[y][x] = waterMap[y][x] * 1.1f * positionDistribution(randomizer);
    }
}

void ShoreGenerator::smoothMap()
{
  float waterLevel = WATER_LEVEL + HEIGHT_SMOOTH_OFFSET;
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
  std::uniform_real_distribution<float> distribution(MIN_HEIGHT_RANDOMIZE_OFFSET, MAX_HEIGHT_RANDOMIZE_OFFSET);
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
          if (map[y-1][x-1] < thresholdValue &&
              map[y-1][x] < thresholdValue &&
              map[y-1][x+1] < thresholdValue &&
              map[y][x-1] < thresholdValue &&
              map[y][x] < thresholdValue &&
              map[y][x+1] < thresholdValue &&
              map[y+1][x-1] < thresholdValue &&
              map[y+1][x] < thresholdValue &&
              map[y+1][x+1] < thresholdValue)
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
  const size_t VERTEX_DATA_LENGTH = tiles.size() * UNIQUE_VERTICES_PER_TILE * ShoreVertex::NUMBER_OF_ELEMENTS;
  const size_t INDICES_DATA_LENGTH = tiles.size() * VERTICES_PER_QUAD;
  size_t indicesBufferIndex = 0;
  std::unique_ptr<GLfloat[]> vertices(new GLfloat[VERTEX_DATA_LENGTH]);
  std::unique_ptr<GLuint[]> indices(new GLuint[INDICES_DATA_LENGTH]);
  for (unsigned int tileIndex = 0; tileIndex < tiles.size(); tileIndex++)
    {
      TerrainTile& tile = tiles[tileIndex];
      int x = tile.mapX, y = tile.mapY;

      ShoreVertex lowLeft(vec3(x - 1, tile.lowLeft, y), vec2(0.0f), normalMap[y][x-1]);
      ShoreVertex lowRight(vec3(x, tile.lowRight, y), vec2(1.0f, 0.0f), normalMap[y][x]);
      ShoreVertex upRight(vec3(x, tile.upperRight, y - 1), vec2(1.0f), normalMap[y-1][x]);
      ShoreVertex upLeft(vec3(x - 1, tile.upperLeft, y - 1), vec2(0.0f, 1.0f), normalMap[y-1][x-1]);

      int vertexBufferOffset = tileIndex * UNIQUE_VERTICES_PER_TILE * ShoreVertex::NUMBER_OF_ELEMENTS;
      bufferVertex(vertices.get(), vertexBufferOffset,    lowLeft);
      bufferVertex(vertices.get(), vertexBufferOffset+8,  lowRight);
      bufferVertex(vertices.get(), vertexBufferOffset+16, upRight);
      bufferVertex(vertices.get(), vertexBufferOffset+24, upLeft);

      GLuint indicesBufferBaseVertex = tileIndex * UNIQUE_VERTICES_PER_TILE;
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + 0;
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + 1;
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + 2;
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + 2;
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + 3;
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + 0;
    }

  basicGLBuffers.bind(VAO | VBO | EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * VERTEX_DATA_LENGTH, vertices.get(), GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * INDICES_DATA_LENGTH, indices.get(), GL_STATIC_DRAW);

  const size_t SIZE_OF_SHORE_VERTEX = ShoreVertex::NUMBER_OF_ELEMENTS * sizeof(GLfloat);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SIZE_OF_SHORE_VERTEX, 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, SIZE_OF_SHORE_VERTEX, (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, SIZE_OF_SHORE_VERTEX, (void*)(5 * sizeof(GLfloat)));
  BufferCollection::bindZero(VAO | VBO | EBO);
}

void ShoreGenerator::bufferVertex(GLfloat *vertices, int offset, ShoreVertex vertex)
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
