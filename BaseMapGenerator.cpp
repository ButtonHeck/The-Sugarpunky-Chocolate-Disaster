#include "BaseMapGenerator.h"

BaseMapGenerator::BaseMapGenerator(std::vector<std::vector<float> > &waterMap, std::vector<std::vector<float> > &hillMap)
  :
    MapGenerator(),
    waterMap(waterMap),
    hillMap(hillMap)
{
  baseChunkTiles.reserve(NUM_TILES / CHUNK_SIZE * CHUNK_SIZE);
  initializeMap(chunkMap);
}

void BaseMapGenerator::prepareMap(bool randomizeShoreFlag)
{
  generateMap();
  smoothMap();
  if (randomizeShoreFlag)
    randomizeShore();
  compressMap(2.0f);
  correctMapAtEdges();
  splitMapToChunks(CHUNK_SIZE);
  baseChunkTiles.shrink_to_fit();
  removeUnderwaterTiles(UNDERWATER_REMOVAL_LEVEL);
  tiles.shrink_to_fit();
  split1x1Tiles(CHUNK_SIZE);
  createTiles(false, false, map, 0);
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
      vertices[offset] =   -1- HALF_TILES_WIDTH + tile.mapX;
      vertices[offset+1] = tile.lowLeft;
      vertices[offset+2] = - HALF_TILES_HEIGHT + tile.mapY;
      vertices[offset+3] = 0.0f;
      vertices[offset+4] = 0.0f;
      vertices[offset+5] = normal1.x;
      vertices[offset+6] = normal1.y;
      vertices[offset+7] = normal1.z;
      //lr1
      vertices[offset+8] =  - HALF_TILES_WIDTH + tile.mapX;
      vertices[offset+9] =  tile.lowRight;
      vertices[offset+10] = - HALF_TILES_HEIGHT + tile.mapY;
      vertices[offset+11] = 1.0f;
      vertices[offset+12] = 0.0f;
      vertices[offset+13] = normal1.x;
      vertices[offset+14] = normal1.y;
      vertices[offset+15] = normal1.z;
      //ur1
      vertices[offset+16] = - HALF_TILES_WIDTH + tile.mapX;
      vertices[offset+17] = tile.upperRight;
      vertices[offset+18] = -1 - HALF_TILES_HEIGHT + tile.mapY;
      vertices[offset+19] = 1.0f;
      vertices[offset+20] = 1.0f;
      vertices[offset+21] = normal1.x;
      vertices[offset+22] = normal1.y;
      vertices[offset+23] = normal1.z;
      //ur2
      vertices[offset+24] = - HALF_TILES_WIDTH + tile.mapX;
      vertices[offset+25] = tile.upperRight;
      vertices[offset+26] = -1 - HALF_TILES_HEIGHT + tile.mapY;
      vertices[offset+27] = 1.0f;
      vertices[offset+28] = 1.0f;
      vertices[offset+29] = normal2.x;
      vertices[offset+30] = normal2.y;
      vertices[offset+31] = normal2.z;
      //ul2
      vertices[offset+32] = -1 - HALF_TILES_WIDTH + tile.mapX;
      vertices[offset+33] = tile.upperLeft;
      vertices[offset+34] = -1 - HALF_TILES_HEIGHT + tile.mapY;
      vertices[offset+35] = 0.0f;
      vertices[offset+36] = 1.0f;
      vertices[offset+37] = normal2.x;
      vertices[offset+38] = normal2.y;
      vertices[offset+39] = normal2.z;
      //ll2
      vertices[offset+40] = -1- HALF_TILES_WIDTH + tile.mapX;
      vertices[offset+41] = tile.lowLeft;
      vertices[offset+42] = - HALF_TILES_HEIGHT + tile.mapY;
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
  glGenVertexArrays(1, &instanceVao);
  glGenBuffers(1, &instanceVbo);
  glGenBuffers(1, &instanceEbo);
  glGenBuffers(1, &instanceModelVbo);
  GLfloat baseChunkInstanceVertices[20] = {
      -1.0f, 0.0f,  1.0f, 0.0f,               0.0f,
       1.0f, 0.0f,  1.0f, (float)CHUNK_SIZE,  0.0f,
       1.0f, 0.0f, -1.0f, (float)CHUNK_SIZE,  (float)CHUNK_SIZE,
      -1.0f, 0.0f, -1.0f, 0.0f,               (float)CHUNK_SIZE
  };
  glBindVertexArray(instanceVao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instanceEbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(baseChunkInstanceVertices), baseChunkInstanceVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  NUM_CHUNKS_INSTANCES = baseChunkTiles.size();
  glm::mat4* baseInstanceChunkModels = new glm::mat4[NUM_CHUNKS_INSTANCES];
  for (unsigned int i = 0; i < baseChunkTiles.size(); i++)
    {
      glm::mat4 model;
      TerrainTile& tile = baseChunkTiles[i];
      model = glm::translate(model, glm::vec3(- HALF_TILES_WIDTH + tile.mapX + CHUNK_SIZE / 2, 0.0f, - HALF_TILES_HEIGHT + tile.mapY + CHUNK_SIZE / 2));
      model = glm::scale(model, glm::vec3(CHUNK_SIZE / 2, 0.0f, CHUNK_SIZE / 2));
      baseInstanceChunkModels[i] = model;
    }
  glBindBuffer(GL_ARRAY_BUFFER, instanceModelVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * baseChunkTiles.size(), &baseInstanceChunkModels[0], GL_STATIC_DRAW);
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
      model = glm::translate(model, glm::vec3(- HALF_TILES_WIDTH + tile.mapX, 0.0f, -HALF_TILES_HEIGHT + tile.mapY - 1));
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
}

void BaseMapGenerator::randomizeShore()
{
  std::uniform_real_distribution<float> distribution(-0.24f, 0.24f);
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

void BaseMapGenerator::compressMap(float ratio)
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

void BaseMapGenerator::splitMapToChunks(int chunkSize)
{
  baseChunks.clear();
  unsigned int chunkOffset = 0;
  for (int y = 0; y < TILES_HEIGHT - chunkSize + 1; y += chunkSize)
    {
      for (int x = 0; x < TILES_WIDTH - chunkSize + 1; x += chunkSize)
        {
          bool emptyChunk = true;
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
          if (emptyChunk)
            {
              for (int ydel = y + 1; ydel < y + chunkSize; ydel++)
                {
                  for (int xdel = x + 1; xdel < x + chunkSize; xdel++)
                    {
                      map[ydel][xdel] = 0;
                      chunkMap[ydel][xdel] = DENY_CHUNK_RENDER_VALUE;
                    }
                }
              baseChunkTiles.emplace_back(x, y, 0, 0, 0, 0, false);
              baseChunks.emplace_back(x, x + chunkSize, y, y + chunkSize, chunkOffset);
              ++chunkOffset;
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

void BaseMapGenerator::split1x1Tiles(int chunkSize)
{
  cellChunks.clear();
  unsigned int offset = 0;
  for (unsigned int y = 0; y < TILES_HEIGHT - chunkSize + 1; y += chunkSize)
    {
      for (unsigned int x = 0; x < TILES_WIDTH - chunkSize + 1; x += chunkSize)
        {
          unsigned int instances = 0;
          for (unsigned int y1 = y+1; y1 < y + chunkSize + 1; y1++)
            {
              for (unsigned int x1 = x; x1 < x + chunkSize; x1++)
                {
                  if ((map[y1][x1] == 0
                       && map[y1-1][x1] == 0
                       && map[y1-1][x1+1] == 0
                       && map[y1][x1+1] == 0)
                      &&
                      (chunkMap[y1][x1] != DENY_CHUNK_RENDER_VALUE
                       && chunkMap[y1-1][x1] != DENY_CHUNK_RENDER_VALUE
                       && chunkMap[y1-1][x1+1] != DENY_CHUNK_RENDER_VALUE
                       && chunkMap[y1][x1+1] != DENY_CHUNK_RENDER_VALUE))
                    {
                      cellTiles.emplace_back(x1, y1, 0, 0, 0, 0, false);
                      instances++;
                    }
                }
            }
          if (instances != 0)
            cellChunks.emplace_back(x, x+chunkSize, y, y+chunkSize, offset, instances);
          offset += instances;
        }
    }
}

void BaseMapGenerator::deleteGLObjects()
{
  MapGenerator::deleteGLObjects();
  glDeleteVertexArrays(1, &instanceVao);
  glDeleteBuffers(1, &instanceVbo);
  glDeleteBuffers(1, &instanceEbo);
  glDeleteBuffers(1, &instanceModelVbo);
  glDeleteVertexArrays(1, &cellVao);
  glDeleteBuffers(1, &cellVbo);
  glDeleteBuffers(1, &cellEbo);
  glDeleteBuffers(1, &cellModelVbo);
}

void BaseMapGenerator::drawChunks(Camera &camera)
{
  glBindVertexArray(instanceVao);
  float cameraOnMapX = glm::clamp(camera.getPosition().x, -(float)HALF_TILES_WIDTH, (float)HALF_TILES_WIDTH);
  float cameraOnMapZ = glm::clamp(camera.getPosition().z, -(float)HALF_TILES_HEIGHT, (float)HALF_TILES_HEIGHT);
  int cameraOnMapCoordX = glm::clamp((int)(TILES_WIDTH + cameraOnMapX) - HALF_TILES_WIDTH, 0, TILES_WIDTH - 1);
  int cameraOnMapCoordZ = glm::clamp((int)(TILES_HEIGHT + cameraOnMapZ) - HALF_TILES_HEIGHT, 0, TILES_HEIGHT - 1);
  glm::vec2 cameraPosition = glm::vec2(cameraOnMapX, cameraOnMapZ);
  glm::vec2 viewDirection = glm::normalize(glm::vec2(camera.getDirection().x, camera.getDirection().z));
  float cameraCorrectedFOVDOT = FOV_DOT_PRODUCT - camera.getPosition().y / 20.0f;
  for (unsigned int i = 0; i < baseChunks.size(); i++)
    {
      if (baseChunks[i].containsPoint(cameraOnMapCoordX, cameraOnMapCoordZ))
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1, baseChunks[i].getInstanceOffset());
          continue;
        }
      glm::vec2 directionToChunkUL =  glm::normalize(glm::vec2(baseChunks[i].getLeft() - (float)HALF_TILES_WIDTH, baseChunks[i].getTop() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkUL, viewDirection) > cameraCorrectedFOVDOT)
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1, baseChunks[i].getInstanceOffset());
          continue;
        }
      glm::vec2 directionToChunkUR =  glm::normalize(glm::vec2(baseChunks[i].getRight() - (float)HALF_TILES_WIDTH, baseChunks[i].getTop() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkUR, viewDirection) > cameraCorrectedFOVDOT)
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1, baseChunks[i].getInstanceOffset());
          continue;
        }
      glm::vec2 directionToChunkLR =  glm::normalize(glm::vec2(baseChunks[i].getRight() - (float)HALF_TILES_WIDTH, baseChunks[i].getBottom() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkLR, viewDirection) > cameraCorrectedFOVDOT)
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1, baseChunks[i].getInstanceOffset());
          continue;
        }
      glm::vec2 directionToChunkLL =  glm::normalize(glm::vec2(baseChunks[i].getLeft() - (float)HALF_TILES_WIDTH, baseChunks[i].getBottom() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkLL, viewDirection) > cameraCorrectedFOVDOT)
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1, baseChunks[i].getInstanceOffset());
    }
}

void BaseMapGenerator::drawCells(Camera &camera)
{
  glBindVertexArray(cellVao);
  float cameraOnMapX = glm::clamp(camera.getPosition().x, -(float)HALF_TILES_WIDTH, (float)HALF_TILES_WIDTH);
  float cameraOnMapZ = glm::clamp(camera.getPosition().z, -(float)HALF_TILES_HEIGHT, (float)HALF_TILES_HEIGHT);
  int cameraOnMapCoordX = glm::clamp((int)(TILES_WIDTH + cameraOnMapX) - HALF_TILES_WIDTH, 0, TILES_WIDTH - 1);
  int cameraOnMapCoordZ = glm::clamp((int)(TILES_HEIGHT + cameraOnMapZ) - HALF_TILES_HEIGHT, 0, TILES_HEIGHT - 1);
  glm::vec2 cameraPosition = glm::vec2(cameraOnMapX, cameraOnMapZ);
  glm::vec2 viewDirection = glm::normalize(glm::vec2(camera.getDirection().x, camera.getDirection().z));
  float cameraCorrectedFOVDOT = FOV_DOT_PRODUCT - camera.getPosition().y / 20.0f;
  for (unsigned int i = 0; i < cellChunks.size(); i++)
    {
      if (cellChunks[i].containsPoint(cameraOnMapCoordX, cameraOnMapCoordZ))
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
          continue;
        }
      glm::vec2 directionToChunkUL =  glm::normalize(glm::vec2(cellChunks[i].getLeft() - (float)HALF_TILES_WIDTH, cellChunks[i].getTop() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkUL, viewDirection) > cameraCorrectedFOVDOT)
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
          continue;
        }
      glm::vec2 directionToChunkUR =  glm::normalize(glm::vec2(cellChunks[i].getRight() - (float)HALF_TILES_WIDTH, cellChunks[i].getTop() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkUR, viewDirection) > cameraCorrectedFOVDOT)
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
          continue;
        }
      glm::vec2 directionToChunkLR =  glm::normalize(glm::vec2(cellChunks[i].getRight() - (float)HALF_TILES_WIDTH, cellChunks[i].getBottom() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkLR, viewDirection) > cameraCorrectedFOVDOT)
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
          continue;
        }
      glm::vec2 directionToChunkLL =  glm::normalize(glm::vec2(cellChunks[i].getLeft() - (float)HALF_TILES_WIDTH, cellChunks[i].getBottom() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkLL, viewDirection) > cameraCorrectedFOVDOT)
        glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
    }
}

std::vector<TerrainTile> &BaseMapGenerator::getChunkTiles()
{
  return baseChunkTiles;
}

std::vector<TerrainTile> &BaseMapGenerator::getCellTiles()
{
  return cellTiles;
}

GLuint &BaseMapGenerator::getChunkVAO()
{
  return instanceVao;
}

GLuint &BaseMapGenerator::getCellVAO()
{
  return cellVao;
}

std::vector<std::vector<float> > &BaseMapGenerator::getChunkMap()
{
  return chunkMap;
}

int BaseMapGenerator::getNumChunksInstances()
{
  return NUM_CHUNKS_INSTANCES;
}

int BaseMapGenerator::getNumCellInstances()
{
  return NUM_CELL_INSTANCES;
}
