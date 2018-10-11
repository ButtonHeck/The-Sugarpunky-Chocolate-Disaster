#include "generator/BaseMapGenerator.h"

BaseMapGenerator::BaseMapGenerator()
  :
    MapGenerator()
{
  randomizer.seed(std::chrono::system_clock::now().time_since_epoch().count());
  squareTiles.reserve(NUM_TILES / CHUNK_SIZE * CHUNK_SIZE);
  initializeMap(chunkMap);
  glCreateVertexArrays(1, &squareVao);
  glCreateBuffers(1, &squareVbo);
  glCreateBuffers(1, &squareEbo);
  glCreateBuffers(1, &squareModelVbo);
  glCreateVertexArrays(1, &cellVao);
  glCreateBuffers(1, &cellVbo);
  glCreateBuffers(1, &cellEbo);
  glCreateBuffers(1, &cellModelVbo);
  glCreateBuffers(1, &cellMultiDE_I_DIBO);
}

BaseMapGenerator::~BaseMapGenerator()
{
  glDeleteVertexArrays(1, &squareVao);
  glDeleteBuffers(1, &squareVbo);
  glDeleteBuffers(1, &squareEbo);
  glDeleteBuffers(1, &squareModelVbo);
  glDeleteVertexArrays(1, &cellVao);
  glDeleteBuffers(1, &cellVbo);
  glDeleteBuffers(1, &cellEbo);
  glDeleteBuffers(1, &cellModelVbo);
  glDeleteBuffers(1, &cellMultiDE_I_DIBO);
}

void BaseMapGenerator::setup(std::vector<std::vector<float> > &shoreMap)
{
  generateMap(shoreMap);
  squareTiles.clear();
  cellTiles.clear();
  splitSquareChunks(CHUNK_SIZE);
  squareTiles.shrink_to_fit();
  splitCellChunks(CHUNK_SIZE);
  fillSquareBufferData();
  fillCellBufferData();
}

void BaseMapGenerator::generateMap(std::vector<std::vector<float> >& shoreMap)
{
  for (unsigned int y = 0; y <= WORLD_HEIGHT; y++)
    {
      for (unsigned int x = 0; x <= WORLD_WIDTH; x++)
        map[y][x] = shoreMap[y][x];
    }
}

void BaseMapGenerator::splitSquareChunks(int chunkSize)
{
  squareChunks.clear();
  unsigned int chunkOffset = 0;
  for (int y = 0; y < WORLD_HEIGHT - chunkSize + 1; y += chunkSize)
    {
      for (int x = 0; x < WORLD_WIDTH - chunkSize + 1; x += chunkSize)
        {
          bool emptyChunk = true;
          for (int y1 = y; y1 < y + chunkSize; y1++)
            {
              for (int x1 = x; x1 < x + chunkSize; x1++)
                {
                  if (map[y1][x1] != 0 || map[y1+1][x1] != 0 || map[y1+1][x1+1] != 0 || map[y1][x1+1] != 0)
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
                      chunkMap[ydel][xdel] = CHUNK_NO_RENDER_VALUE;
                    }
                }
              squareTiles.emplace_back(x, y, 0, 0, 0, 0, false);
              squareChunks.emplace_back(x, x + chunkSize, y, y + chunkSize, chunkOffset, 1);
              ++chunkOffset;
            }
        }
    }
}

void BaseMapGenerator::splitCellChunks(int chunkSize)
{
  cellChunks.clear();
  unsigned int offset = 0;
  for (int y = 0; y < WORLD_HEIGHT - chunkSize + 1; y += chunkSize)
    {
      for (int x = 0; x < WORLD_WIDTH - chunkSize + 1; x += chunkSize)
        {
          unsigned int instances = 0;
          for (int y1 = y+1; y1 < y + chunkSize + 1; y1++)
            {
              for (int x1 = x; x1 < x + chunkSize; x1++)
                {
                  if ((map[y1][x1] == 0
                       && map[y1-1][x1] == 0
                       && map[y1-1][x1+1] == 0
                       && map[y1][x1+1] == 0)
                      &&
                      (chunkMap[y1][x1] != CHUNK_NO_RENDER_VALUE
                       && chunkMap[y1-1][x1] != CHUNK_NO_RENDER_VALUE
                       && chunkMap[y1-1][x1+1] != CHUNK_NO_RENDER_VALUE
                       && chunkMap[y1][x1+1] != CHUNK_NO_RENDER_VALUE))
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

void BaseMapGenerator::fillSquareBufferData()
{
  constexpr GLfloat CHUNK_VERTICES[20] = {
      -1.0f, 0.0f,  1.0f, 0.0f,               0.0f,
       1.0f, 0.0f,  1.0f, (float)CHUNK_SIZE,  0.0f,
       1.0f, 0.0f, -1.0f, (float)CHUNK_SIZE,  (float)CHUNK_SIZE,
      -1.0f, 0.0f, -1.0f, 0.0f,               (float)CHUNK_SIZE
  };
  glBindVertexArray(squareVao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, squareVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(CHUNK_VERTICES), CHUNK_VERTICES, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  NUM_SQUARE_INSTANCES = squareTiles.size();
  glm::mat4* baseInstanceChunkModels = new glm::mat4[NUM_SQUARE_INSTANCES];
  for (unsigned int i = 0; i < squareTiles.size(); i++)
    {
      glm::mat4 model;
      TerrainTile& tile = squareTiles[i];
      model = glm::translate(model, glm::vec3(- HALF_WORLD_WIDTH + tile.mapX + CHUNK_SIZE / 2, 0.0f, - HALF_WORLD_HEIGHT + tile.mapY + CHUNK_SIZE / 2));
      model = glm::scale(model, glm::vec3(CHUNK_SIZE / 2, 0.0f, CHUNK_SIZE / 2));
      baseInstanceChunkModels[i] = model;
    }
  glBindBuffer(GL_ARRAY_BUFFER, squareModelVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * squareTiles.size(), &baseInstanceChunkModels[0], GL_STATIC_DRAW);
  for (unsigned int i = 0; i < 4; ++i)
    {
      glEnableVertexAttribArray(i+3);
      glVertexAttribPointer(i+3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
      glVertexAttribDivisor(i+3, 1);
    }
  resetAllGLBuffers();
  delete[] baseInstanceChunkModels;
}

void BaseMapGenerator::fillCellBufferData()
{
  constexpr GLfloat CELL_VERTICES[20] = {
       0.0f, 0.0f,  1.0f, 0.0f,  0.0f,
       1.0f, 0.0f,  1.0f, 1.0f,  0.0f,
       1.0f, 0.0f,  0.0f, 1.0f,  1.0f,
       0.0f, 0.0f,  0.0f, 0.0f,  1.0f
  };
  glBindVertexArray(cellVao);
  glBindBuffer(GL_DRAW_INDIRECT_BUFFER, cellMultiDE_I_DIBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cellEbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, cellVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(CELL_VERTICES), CELL_VERTICES, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glm::mat4* cellInstanceModels = new glm::mat4[cellTiles.size()];
  for (unsigned int i = 0; i < cellTiles.size(); i++)
    {
      glm::mat4 model;
      TerrainTile& tile = cellTiles[i];
      model = glm::translate(model, glm::vec3(- HALF_WORLD_WIDTH + tile.mapX, 0.0f, -HALF_WORLD_HEIGHT + tile.mapY - 1));
      cellInstanceModels[i] = model;
    }

  glBindBuffer(GL_ARRAY_BUFFER, cellModelVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * cellTiles.size(), &cellInstanceModels[0], GL_STATIC_DRAW);
  for (unsigned int i = 0; i < 4; ++i)
    {
      glEnableVertexAttribArray(i+3);
      glVertexAttribPointer(i+3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
      glVertexAttribDivisor(i+3, 1);
    }
  delete[] cellInstanceModels;
  resetAllGLBuffers();
}

std::vector<TerrainChunk> &BaseMapGenerator::getCellChunks()
{
  return cellChunks;
}

GLuint &BaseMapGenerator::getSquareVAO()
{
  return squareVao;
}

GLuint &BaseMapGenerator::getCellVAO()
{
  return cellVao;
}

GLuint &BaseMapGenerator::getCellDIBO()
{
  return cellMultiDE_I_DIBO;
}

int BaseMapGenerator::getNumSquareInstances() const
{
  return NUM_SQUARE_INSTANCES;
}

void BaseMapGenerator::deserialize(std::ifstream &input)
{
  MapGenerator::deserialize(input);
  for (std::vector<float>& row : chunkMap)
    {
      for (float& value : row)
        value = 0;
    }
}
