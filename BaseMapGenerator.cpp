#include "BaseMapGenerator.h"

BaseMapGenerator::BaseMapGenerator(std::vector<std::vector<float> > &waterMap, std::vector<std::vector<float> > &hillMap)
  :
    MapGenerator(),
    waterMap(waterMap),
    hillMap(hillMap)
{
  for (int i = 0; i < NUM_BASE_TERRAIN_CHUNKS; i++)
    baseChunkTiles[i].reserve(NUM_TILES / BASE_TERRAIN_CHUNK_SIZES[i] * BASE_TERRAIN_CHUNK_SIZES[i]);
  initializeMap(chunkMap);
}

void BaseMapGenerator::prepareMap()
{
  generateMap();
  smoothMap();
  compressMap(2.0f, true);
  correctMapAtEdges();
  for (int i = 0; i < NUM_BASE_TERRAIN_CHUNKS; i++)
    {
      splitMapToChunks(baseChunkTiles[i], BASE_TERRAIN_CHUNK_SIZES[i], (bool)i);
      baseChunkTiles[i].shrink_to_fit();
    }
  removeUnderwaterTiles(UNDERWATER_REMOVAL_LEVEL);
  tiles.shrink_to_fit();
  split1x1Tiles();
  createTiles(false, false);
}

void BaseMapGenerator::fillBufferData()
{
  GLfloat vertices[tiles.size() * 48];
  glm::vec3 normal1, normal2;
  for (unsigned int c = 0; c < tiles.size(); c++)
    {
      TerrainTile& tile = tiles[c];
      int offset = c * 48;
      normal1 = glm::cross(
            glm::vec3(tile.mapX, tile.upperRight, tile.mapY - 1)
            -
            glm::vec3(tile.mapX, tile.lowRight, tile.mapY)
            ,
            glm::vec3(tile.mapX - 1, tile.lowLeft, tile.mapY)
            -
            glm::vec3(tile.mapX, tile.lowRight, tile.mapY));
      normal2 = glm::cross(
            glm::vec3(tile.mapX - 1, tile.lowLeft, tile.mapY)
            -
            glm::vec3(tile.mapX - 1, tile.upperLeft, tile.mapY - 1)
            ,
            glm::vec3(tile.mapX, tile.upperRight, tile.mapY - 1)
            -
            glm::vec3(tile.mapX - 1, tile.upperLeft, tile.mapY - 1));
      //ll1
      vertices[offset] =   -1- TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+1] = tile.lowLeft;
      vertices[offset+2] = - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+3] = 0.0f;
      vertices[offset+4] = 0.0f;
      vertices[offset+5] = normal1.x;
      vertices[offset+6] = normal1.y;
      vertices[offset+7] = normal1.z;
      //lr1
      vertices[offset+8] =  - TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+9] =  tile.lowRight;
      vertices[offset+10] = - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+11] = 1.0f;
      vertices[offset+12] = 0.0f;
      vertices[offset+13] = normal1.x;
      vertices[offset+14] = normal1.y;
      vertices[offset+15] = normal1.z;
      //ur1
      vertices[offset+16] = - TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+17] = tile.upperRight;
      vertices[offset+18] = -1 - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+19] = 1.0f;
      vertices[offset+20] = 1.0f;
      vertices[offset+21] = normal1.x;
      vertices[offset+22] = normal1.y;
      vertices[offset+23] = normal1.z;
      //ur2
      vertices[offset+24] = - TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+25] = tile.upperRight;
      vertices[offset+26] = -1 - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+27] = 1.0f;
      vertices[offset+28] = 1.0f;
      vertices[offset+29] = normal2.x;
      vertices[offset+30] = normal2.y;
      vertices[offset+31] = normal2.z;
      //ul2
      vertices[offset+32] = -1 - TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+33] = tile.upperLeft;
      vertices[offset+34] = -1 - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+35] = 0.0f;
      vertices[offset+36] = 1.0f;
      vertices[offset+37] = normal2.x;
      vertices[offset+38] = normal2.y;
      vertices[offset+39] = normal2.z;
      //ll2
      vertices[offset+40] = -1- TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+41] = tile.lowLeft;
      vertices[offset+42] = - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+43] = 0.0f;
      vertices[offset+44] = 0.0f;
      vertices[offset+45] = normal2.x;
      vertices[offset+46] = normal2.y;
      vertices[offset+47] = normal2.z;
    }
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
  resetAllGLBuffers();
}

void BaseMapGenerator::fillChunkBufferData()
{
  glGenVertexArrays(NUM_BASE_TERRAIN_CHUNKS, instanceVao);
  glGenBuffers(NUM_BASE_TERRAIN_CHUNKS, instanceVbo);
  glGenBuffers(NUM_BASE_TERRAIN_CHUNKS, instanceEbo);
  glGenBuffers(NUM_BASE_TERRAIN_CHUNKS, instanceModelVbo);
  for (unsigned int vao = 0; vao < NUM_BASE_TERRAIN_CHUNKS; vao++)
    {
      GLfloat baseChunkInstanceVertices[20] = {
          -1.0f, 0.0f,  1.0f, 0.0f,                                  0.0f,
           1.0f, 0.0f,  1.0f, (float)BASE_TERRAIN_CHUNK_SIZES[vao],  0.0f,
           1.0f, 0.0f, -1.0f, (float)BASE_TERRAIN_CHUNK_SIZES[vao],  (float)BASE_TERRAIN_CHUNK_SIZES[vao],
          -1.0f, 0.0f, -1.0f, 0.0f,                                  (float)BASE_TERRAIN_CHUNK_SIZES[vao]
      };
      glBindVertexArray(instanceVao[vao]);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instanceEbo[vao]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, instanceVbo[vao]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(baseChunkInstanceVertices), baseChunkInstanceVertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
      NUM_CHUNKS_INSTANCES[vao] = baseChunkTiles[vao].size();
      glm::mat4* baseInstanceChunkModels = new glm::mat4[NUM_CHUNKS_INSTANCES[vao]];
      std::vector<TerrainTile>& baseTerrainChunk = baseChunkTiles[vao];
      for (unsigned int i = 0; i < baseTerrainChunk.size(); i++)
        {
          glm::mat4 model;
          TerrainTile& tile = baseTerrainChunk[i];
          model = glm::translate(model, glm::vec3(- TILES_WIDTH / 2 + tile.mapX + BASE_TERRAIN_CHUNK_SIZES[vao] / 2, 0.0f, - TILES_HEIGHT / 2 + tile.mapY + BASE_TERRAIN_CHUNK_SIZES[vao] / 2));
          model = glm::scale(model, glm::vec3(BASE_TERRAIN_CHUNK_SIZES[vao] / 2, 0.0f, BASE_TERRAIN_CHUNK_SIZES[vao] / 2));
          baseInstanceChunkModels[i] = model;
        }
      glBindBuffer(GL_ARRAY_BUFFER, instanceModelVbo[vao]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * baseTerrainChunk.size(), &baseInstanceChunkModels[0], GL_STATIC_DRAW);
      glEnableVertexAttribArray(3);
      glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), 0);
      glEnableVertexAttribArray(4);
      glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(sizeof(glm::vec4)));
      glEnableVertexAttribArray(5);
      glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
      glEnableVertexAttribArray(6);
      glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
      glVertexAttribDivisor(3, 1);
      glVertexAttribDivisor(4, 1);
      glVertexAttribDivisor(5, 1);
      glVertexAttribDivisor(6, 1);
      resetAllGLBuffers();
      delete[] baseInstanceChunkModels;
    }
}

void BaseMapGenerator::fillCellBufferData()
{
  glGenVertexArrays(1, &cellVao);
  glGenBuffers(1, &cellVbo);
  glGenBuffers(1, &cellEbo);
  glGenBuffers(1, &cellModelVbo);
  GLfloat cellVertices[20] = {
       0.0f, 0.0f,  1.0f, 0.0f,  0.0f,
       1.0f, 0.0f,  1.0f, 1.0f,  0.0f,
       1.0f, 0.0f,  0.0f, 1.0f,  1.0f,
       0.0f, 0.0f,  0.0f, 0.0f,  1.0f
  };
    glBindVertexArray(cellVao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cellEbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, cellVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cellVertices), cellVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  NUM_CELL_INSTANCES = cellTiles.size();
  glm::mat4* cellInstanceModels = new glm::mat4[NUM_CELL_INSTANCES];
  for (unsigned int i = 0; i < cellTiles.size(); i++)
    {
      glm::mat4 model;
      TerrainTile& tile = cellTiles[i];
      model = glm::translate(model, glm::vec3(-TILES_WIDTH / 2 + tile.mapX, 0.0f, -TILES_HEIGHT / 2 + tile.mapY - 1));
      cellInstanceModels[i] = model;
    }
  glBindBuffer(GL_ARRAY_BUFFER, cellModelVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * cellTiles.size(), &cellInstanceModels[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), 0);
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(sizeof(glm::vec4)));
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
  glEnableVertexAttribArray(6);
  glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
  glVertexAttribDivisor(3, 1);
  glVertexAttribDivisor(4, 1);
  glVertexAttribDivisor(5, 1);
  glVertexAttribDivisor(6, 1);
  delete[] cellInstanceModels;
  resetAllGLBuffers();
}

void BaseMapGenerator::generateMap()
{
  std::uniform_real_distribution<float> distribution(0.9f, 1.1f);
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
  //slightly randomize shore line between base and water level
  std::uniform_real_distribution<float> distribution(-0.2f, 0.2f);
  for (unsigned int y = 0; y < TILES_HEIGHT; y++)
    {
      for (unsigned int x = 0; x < TILES_WIDTH; x++)
        {
          if (map[y][x] < 0)
            map[y][x] += distribution(randomizer);
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

void BaseMapGenerator::split1x1Tiles()
{
  for (unsigned int y = 1; y <= TILES_HEIGHT; y++)
    {
      for (unsigned int x = 0; x < TILES_WIDTH; x++)
        {
          if ((map[y][x] == 0
               && map[y-1][x] == 0
               && map[y-1][x+1] == 0
               && map[y][x+1] == 0)
              &&
              (chunkMap[y][x] != DENY_CHUNK_RENDER_VALUE
               && chunkMap[y-1][x] != DENY_CHUNK_RENDER_VALUE
               && chunkMap[y-1][x+1] != DENY_CHUNK_RENDER_VALUE
               && chunkMap[y][x+1] != DENY_CHUNK_RENDER_VALUE))
            {
              cellTiles.emplace_back(x, y, 0, 0, 0, 0, false);
            }
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
  glDeleteVertexArrays(1, &cellVao);
  glDeleteBuffers(1, &cellVbo);
  glDeleteBuffers(1, &cellEbo);
  glDeleteBuffers(1, &cellModelVbo);
}

std::vector<TerrainTile> &BaseMapGenerator::getChunkTiles(int i)
{
  return baseChunkTiles[i];
}

GLuint &BaseMapGenerator::getChunkVAO(int i)
{
  return instanceVao[i];
}

GLuint &BaseMapGenerator::getCellVAO()
{
  return cellVao;
}

int BaseMapGenerator::getNumChunksInstances(int i)
{
  return NUM_CHUNKS_INSTANCES[i];
}

int BaseMapGenerator::getNumCellInstances()
{
  return NUM_CELL_INSTANCES;
}
