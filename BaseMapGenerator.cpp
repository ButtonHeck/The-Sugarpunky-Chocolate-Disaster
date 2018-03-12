#include "BaseMapGenerator.h"

BaseMapGenerator::BaseMapGenerator(std::vector<std::vector<float> > &waterMap, std::vector<std::vector<float> > &hillMap)
  :
    MapGenerator(),
    waterMap(waterMap),
    hillMap(hillMap)
{
  for (int i = 0; i < 5; i++)
    baseChunkTiles[i].reserve(NUM_TILES / BASE_TERRAIN_CHUNK_SIZES[i] * BASE_TERRAIN_CHUNK_SIZES[i]);
  initializeMap(chunkMap);
}

void BaseMapGenerator::prepareMap()
{
  generateMap();
  smoothMap();
  compressMap(2.0f, true);
  correctMapAtEdges();
  for (int i = 0; i < 5; i++)
    {
      splitMapToChunks(baseChunkTiles[i], BASE_TERRAIN_CHUNK_SIZES[i], (bool)i);
      baseChunkTiles[i].shrink_to_fit();
    }
  denyMapInvisibleTiles();
  removeUnderwaterTiles(UNDERWATER_REMOVAL_LEVEL);
  tiles.shrink_to_fit();
  createTiles(false, false);
}

void BaseMapGenerator::fillBufferData()
{
  GLfloat vertices[tiles.size() * 20];
  GLuint indices[tiles.size() * 6];
  for (unsigned int c = 0; c < tiles.size(); c++)
    {
      TerrainTile& tile = tiles[c];
      int offset = c * 20;
      int indexArrayOffset = c * 6;
      int index = c * 4;
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
      vertices[offset+8] = 1.0f;
      vertices[offset+9] = 0.0f;
      //ur
      vertices[offset+10] = - TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+11] = tile.upperRight;
      vertices[offset+12] = -1 - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+13] = 1.0f;
      vertices[offset+14] = 1.0f;
      //ul
      vertices[offset+15] = -1 - TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+16] = tile.upperLeft;
      vertices[offset+17] = -1 - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+18] = 0.0f;
      vertices[offset+19] = 1.0f;

      indices[indexArrayOffset] = index;
      indices[indexArrayOffset+1] = index + 1;
      indices[indexArrayOffset+2] = index + 2;
      indices[indexArrayOffset+3] = index + 2;
      indices[indexArrayOffset+4] = index + 3;
      indices[indexArrayOffset+5] = index;
    }
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  setupGLBuffersAttributes();
  resetAllGLBuffers();
}

void BaseMapGenerator::fillChunkBufferData()
{
  glGenVertexArrays(5, instanceVao);
  glGenBuffers(5, instanceVbo);
  glGenBuffers(5, instanceEbo);
  glGenBuffers(5, instanceModelVbo);
  for (unsigned int vao = 0; vao < 5; vao++)
    {
      GLfloat baseChunkInstanceVertices[20] = {
          -1.0f, 0.0f,  1.0f, 0.0f,                                      0.0f,
           1.0f, 0.0f,  1.0f, (float)BASE_TERRAIN_CHUNK_SIZES[vao] - 1,  0.0f,
           1.0f, 0.0f, -1.0f, (float)BASE_TERRAIN_CHUNK_SIZES[vao] - 1,  (float)BASE_TERRAIN_CHUNK_SIZES[vao] - 1,
          -1.0f, 0.0f, -1.0f, 0.0f,                                      (float)BASE_TERRAIN_CHUNK_SIZES[vao] - 1
      };
      glBindVertexArray(instanceVao[vao]);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instanceEbo[vao]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, instanceVbo[vao]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(baseChunkInstanceVertices), baseChunkInstanceVertices, GL_STATIC_DRAW);
      setupGLBuffersAttributes();
      NUM_INSTANCES[vao] = baseChunkTiles[vao].size();
      glm::mat4* baseInstanceChunkModels = new glm::mat4[NUM_INSTANCES[vao]];
      std::vector<TerrainTile>& baseTerrainChunk = baseChunkTiles[vao];
      for (unsigned int i = 0; i < baseTerrainChunk.size(); i++)
        {
          glm::mat4 model;
          TerrainTile& tile = baseTerrainChunk[i];
          model = glm::translate(model, glm::vec3(- TILES_WIDTH / 2 + tile.mapX + BASE_TERRAIN_CHUNK_SIZES[vao] / 2 - 0.5f, 0.0f, - TILES_HEIGHT / 2 + tile.mapY + BASE_TERRAIN_CHUNK_SIZES[vao] / 2 - 0.5f));
          model = glm::scale(model, glm::vec3(BASE_TERRAIN_CHUNK_SIZES[vao] / 2 - 0.5f, 0.0f, BASE_TERRAIN_CHUNK_SIZES[vao] / 2 - 0.5f));
          baseInstanceChunkModels[i] = model;
        }
      glBindBuffer(GL_ARRAY_BUFFER, instanceModelVbo[vao]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * baseTerrainChunk.size(), &baseInstanceChunkModels[0], GL_STATIC_DRAW);
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), 0);
      glEnableVertexAttribArray(3);
      glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(sizeof(glm::vec4)));
      glEnableVertexAttribArray(4);
      glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
      glEnableVertexAttribArray(5);
      glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
      glVertexAttribDivisor(2, 1);
      glVertexAttribDivisor(3, 1);
      glVertexAttribDivisor(4, 1);
      glVertexAttribDivisor(5, 1);
      resetAllGLBuffers();
      delete[] baseInstanceChunkModels;
    }
}

void BaseMapGenerator::generateMap()
{
  std::uniform_real_distribution<float> distribution(0.9f, 1.1f);
  std::default_random_engine randomizer;
  for (unsigned int y = 0; y <= TILES_HEIGHT; y++)
    {
      for (unsigned int x = 0; x <= TILES_WIDTH; x++)
        {
          map[y][x] = waterMap[y][x] * 1.1f * distribution(randomizer);
        }
    }
}

void BaseMapGenerator::smoothMap()
{
  //smooth tile below on map
  for (unsigned int y = 1; y < TILES_HEIGHT - 1; y++)
    {
      for (unsigned int x = 1; x < TILES_WIDTH; x++)
        {
          if (map[y-1][x] < WATER_LEVEL - WATER_LEVEL / 4)
            map[y][x] += WATER_LEVEL / 1.5f;
        }
    }
  //smooth tile upper on map
  for (unsigned int y = 2; y < TILES_HEIGHT; y++)
    {
      for (unsigned int x = 1; x < TILES_WIDTH; x++)
        {
          if (map[y+1][x] < WATER_LEVEL - WATER_LEVEL / 4)
            map[y][x] += WATER_LEVEL / 1.5f;
        }
    }
  //smooth tile left on map
  for (unsigned int y = 1; y < TILES_HEIGHT; y++)
    {
      for (unsigned int x = 2; x < TILES_WIDTH; x++)
        {
          if (map[y][x+1] < WATER_LEVEL - WATER_LEVEL / 4)
            map[y][x] += WATER_LEVEL / 1.5f;
        }
    }
  //smooth tile right on map
  for (unsigned int y = 1; y < TILES_HEIGHT; y++)
    {
      for (unsigned int x = 1; x < TILES_WIDTH - 1; x++)
        {
          if (map[y][x-1] < WATER_LEVEL - WATER_LEVEL / 4)
            map[y][x] += WATER_LEVEL / 1.5f;
        }
    }
}

void BaseMapGenerator::correctMapAtEdges()
{
  //correct top and bottom sides of the map
  for (unsigned int x = 0; x < TILES_WIDTH; ++x)
    {
      if (waterMap[0][x] != 0)
        map[0][x] = map[1][x];
      if (waterMap[TILES_HEIGHT][x] != 0)
        map[TILES_HEIGHT][x] = map[TILES_HEIGHT - 1][x];
    }
  //correct left and right sides of the map
  for (unsigned int y = 0; y < TILES_HEIGHT; ++y)
    {
      if (waterMap[y][0] != 0)
        map[y][0] = map[y][1];
      if (waterMap[y][TILES_WIDTH] != 0)
        map[y][TILES_WIDTH] = map[y][TILES_WIDTH - 1];
    }
}

void BaseMapGenerator::compressMap(float ratio, bool entireRange)
{
  for (std::vector<float>& row : map)
    {
      for (float& height : row)
        {
          if (height < 0 || entireRange)
            height /= ratio;
        }
    }
}

void BaseMapGenerator::denyMapInvisibleTiles()
{
  for (unsigned int y = 1; y < TILES_HEIGHT - 1; y++)
    {
      for (unsigned int x = 1; x < TILES_WIDTH - 1; x++)
        {
          if (hillMap[y][x] != 0
              && hillMap[y-1][x-1] != 0
              && hillMap[y-1][x] != 0
              && hillMap[y-1][x+1] != 0
              && hillMap[y][x-1] != 0
              && hillMap[y][x+1] != 0
              && hillMap[y+1][x-1] != 0
              && hillMap[y+1][x] != 0
              && hillMap[y+1][x+1] != 0)
            {
              map[y][x] = DENY_TILE_RENDER_VALUE;
            }
        }
    }
}

void BaseMapGenerator::splitMapToChunks(std::vector<TerrainTile> &baseChunks, int chunkSize, bool overlap)
{
  int step = overlap ? chunkSize / 2 : chunkSize - 1;
  for (int y = 0; y < TILES_HEIGHT - step - 1; y += step)
    {
      for (int x = 0; x < TILES_WIDTH - step - 1; x += step)
        {
          bool emptyChunk = true, chunked = true;
          if (overlap)
            {
              for (int y1 = y+1; y1 < y + chunkSize; y1++)
                {
                  for (int x1 = x+1; x1 < x + chunkSize; x1++)
                    {
                      if (chunkMap[y1][x1] != DENY_CHUNK_RENDER_VALUE
                          || chunkMap[y1+1][x1] != DENY_CHUNK_RENDER_VALUE
                          || chunkMap[y1][x1+1] != DENY_CHUNK_RENDER_VALUE
                          || chunkMap[y1+1][x1+1] != DENY_CHUNK_RENDER_VALUE)
                        {
                          chunked = false;
                          break;
                        }
                    }
                }
            }
          for (int y1 = y; y1 < y + chunkSize; y1++)
            {
              for (int x1 = x; x1 < x + chunkSize; x1++)
                {
                  if ((map[y1][x1] != 0 && map[y1][x1] != DENY_TILE_RENDER_VALUE)
                      || (map[y1+1][x1] != 0 && map[y1+1][x1] != DENY_TILE_RENDER_VALUE)
                      || (map[y1+1][x1+1] != 0 && map[y1+1][x1+1] != DENY_TILE_RENDER_VALUE)
                      || (map[y1][x1+1] != 0 && map[y1][x1+1] != DENY_TILE_RENDER_VALUE))
                    {
                      emptyChunk = false;
                      break;
                    }
                }
              if (!emptyChunk)
                break;
            }
          if (emptyChunk && (!chunked || !overlap))
            {
              for (int ydel = y + 1; ydel < y + chunkSize; ydel++)
                {
                  for (int xdel = x + 1; xdel < x + chunkSize; xdel++)
                    {
                      map[ydel][xdel] = 0;
                      chunkMap[ydel][xdel] = DENY_CHUNK_RENDER_VALUE;
                    }
                }
              baseChunks.emplace_back(x, y, 0, 0, 0, 0, false);
            }
        }
    }
}

void BaseMapGenerator::removeUnderwaterTiles(float thresholdValue)
{
  for (unsigned int y = 1; y < TILES_HEIGHT - 1; y++)
    {
      for (unsigned int x = 1; x < TILES_WIDTH - 1; x++)
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
            map[y][x] = DENY_TILE_RENDER_VALUE;
        }
    }
}

void BaseMapGenerator::deleteGLObjects()
{
  MapGenerator::deleteGLObjects();
  glDeleteVertexArrays(5, instanceVao);
  glDeleteBuffers(5, instanceVbo);
  glDeleteBuffers(5, instanceEbo);
  glDeleteBuffers(5, instanceModelVbo);
}

std::vector<TerrainTile> &BaseMapGenerator::getChunkTiles(int i)
{
  return baseChunkTiles[i];
}

GLuint &BaseMapGenerator::getInstanceVAO(int i)
{
  return instanceVao[i];
}

int BaseMapGenerator::getNumInstances(int i)
{
  return NUM_INSTANCES[i];
}
