#include "LandGenerator"

LandGenerator::LandGenerator()
  :
    Generator(),
    cellBuffers(VAO | VBO | INSTANCE_VBO | EBO | DIBO)
{
  randomizer.seed(std::chrono::system_clock::now().time_since_epoch().count());
  //this collection already have VAO/VBO/EBO in Generator ctor
  basicGLBuffers.add(INSTANCE_VBO);
}

void LandGenerator::setup(const map2D_f &shoreMap)
{
  initializeMap(chunkMap);
  generateMap(shoreMap);
  splitChunks(CHUNK_SIZE);
  tiles.shrink_to_fit();
  splitCellChunks(CHUNK_SIZE);
  fillBufferData();
  fillCellBufferData();
}

void LandGenerator::generateMap(const map2D_f &shoreMap)
{
  for (unsigned int y = 0; y <= WORLD_HEIGHT; y++)
    {
      for (unsigned int x = 0; x <= WORLD_WIDTH; x++)
        map[y][x] = shoreMap[y][x];
    }
}

void LandGenerator::splitChunks(int chunkSize)
{
  chunks.clear();
  tiles.clear();
  unsigned int chunkOffset = 0;
  for (int startY = 0; startY < WORLD_HEIGHT - chunkSize + 1; startY += chunkSize)
    {
      for (int startX = 0; startX < WORLD_WIDTH - chunkSize + 1; startX += chunkSize)
        {
          bool emptyChunk = true;
          for (int y = startY; y < startY + chunkSize; y++)
            {
              for (int x = startX; x < startX + chunkSize; x++)
                {
                  if (map[y][x] != 0 || map[y+1][x] != 0 || map[y+1][x+1] != 0 || map[y][x+1] != 0)
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
              for (int noRenderY = startY + 1; noRenderY < startY + chunkSize; noRenderY++)
                {
                  for (int noRenderX = startX + 1; noRenderX < startX + chunkSize; noRenderX++)
                    {
                      map[noRenderY][noRenderX] = 0;
                      chunkMap[noRenderY][noRenderX] = CHUNK_NO_RENDER_VALUE;
                    }
                }
              tiles.emplace_back(startX, startY, 0, 0, 0, 0);
              chunks.emplace_back(startX, startX + chunkSize, startY, startY + chunkSize, chunkOffset, 1);
              ++chunkOffset;
            }
        }
    }
}

void LandGenerator::splitCellChunks(int chunkSize)
{
  cellTiles.clear();
  cellChunks.clear();
  unsigned int chunkOffset = 0;
  for (int startY = 0; startY < WORLD_HEIGHT - chunkSize + 1; startY += chunkSize)
    {
      for (int startX = 0; startX < WORLD_WIDTH - chunkSize + 1; startX += chunkSize)
        {
          unsigned int cellInstances = 0;
          for (int y = startY + 1; y < startY + chunkSize + 1; y++)
            {
              for (int x = startX; x < startX + chunkSize; x++)
                {
                  if (map[y][x] == 0 && map[y-1][x] == 0 && map[y-1][x+1] == 0 && map[y][x+1] == 0 &&
                      chunkMap[y][x] != CHUNK_NO_RENDER_VALUE &&
                      chunkMap[y-1][x] != CHUNK_NO_RENDER_VALUE &&
                      chunkMap[y-1][x+1] != CHUNK_NO_RENDER_VALUE &&
                      chunkMap[y][x+1] != CHUNK_NO_RENDER_VALUE)
                    {
                      cellTiles.emplace_back(x, y, 0, 0, 0, 0);
                      cellInstances++;
                    }
                }
            }
          if (cellInstances != 0)
            cellChunks.emplace_back(startX, startX+chunkSize, startY, startY+chunkSize, chunkOffset, cellInstances);
          chunkOffset += cellInstances;
        }
    }
}

void LandGenerator::fillBufferData()
{
  float halfChunkSize = CHUNK_SIZE / 2;
  const unsigned int CHUNK_VERTICES_SIZE_FLOATS = 20;
  GLfloat chunkVertices[CHUNK_VERTICES_SIZE_FLOATS] = {
      -halfChunkSize, 0.0f,  halfChunkSize, 0.0f,               0.0f,
       halfChunkSize, 0.0f,  halfChunkSize, (float)CHUNK_SIZE,  0.0f,
       halfChunkSize, 0.0f, -halfChunkSize, (float)CHUNK_SIZE,  (float)CHUNK_SIZE,
      -halfChunkSize, 0.0f, -halfChunkSize, 0.0f,               (float)CHUNK_SIZE
  };
  basicGLBuffers.bind(VAO);
  bufferData(basicGLBuffers, chunkVertices, CHUNK_VERTICES_SIZE_FLOATS);
  setupGLBufferAttributes();

  std::unique_ptr<glm::vec4[]> landChunkInstanceTranslations(new glm::vec4[tiles.size()]);
  for (unsigned int tileIndex = 0; tileIndex < tiles.size(); tileIndex++)
    {
      TerrainTile& tile = tiles[tileIndex];
      landChunkInstanceTranslations[tileIndex] = glm::vec4(-HALF_WORLD_WIDTH + tile.mapX + halfChunkSize,
                                                           0.0f,
                                                           -HALF_WORLD_HEIGHT + tile.mapY + halfChunkSize,
                                                           0.0f);
    }
  basicGLBuffers.bind(INSTANCE_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * tiles.size(), &landChunkInstanceTranslations[0], GL_STATIC_DRAW);
  setupGLBufferInstancedAttributes();
  BufferCollection::bindZero(VAO | VBO | EBO);
}

void LandGenerator::fillCellBufferData()
{
  const unsigned int CELL_VERTICES_SIZE_FLOATS = 20;
  GLfloat cellVertices[CELL_VERTICES_SIZE_FLOATS] = {
       0.0f, 0.0f,  1.0f, 0.0f,  0.0f,
       1.0f, 0.0f,  1.0f, 1.0f,  0.0f,
       1.0f, 0.0f,  0.0f, 1.0f,  1.0f,
       0.0f, 0.0f,  0.0f, 0.0f,  1.0f
  };
  cellBuffers.bind(VAO | DIBO);
  bufferData(cellBuffers, cellVertices, CELL_VERTICES_SIZE_FLOATS);
  setupGLBufferAttributes();

  std::unique_ptr<glm::vec4[]> cellInstanceTranslations(new glm::vec4[cellTiles.size()]);
  for (unsigned int tileIndex = 0; tileIndex < cellTiles.size(); tileIndex++)
    {
      TerrainTile& tile = cellTiles[tileIndex];
      cellInstanceTranslations[tileIndex] = glm::vec4(-HALF_WORLD_WIDTH + tile.mapX, 0.0f, -HALF_WORLD_HEIGHT + tile.mapY - 1, 0.0f);
    }
  cellBuffers.bind(INSTANCE_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * cellTiles.size(), &cellInstanceTranslations[0], GL_STATIC_DRAW);
  setupGLBufferInstancedAttributes();
  BufferCollection::bindZero(VAO | VBO | EBO);
}

void LandGenerator::bufferData(BufferCollection& bufferCollection, GLfloat* buffer, size_t size)
{
  bufferCollection.bind(VBO | EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER, sizeof(buffer) * size, buffer, GL_STATIC_DRAW);
}

void LandGenerator::setupGLBufferAttributes()
{
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
}

void LandGenerator::setupGLBufferInstancedAttributes()
{
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
  glVertexAttribDivisor(2, 1);
}

void LandGenerator::updateCellsIndirectBuffer(const Frustum& frustum)
{
  cellBuffers.bind(VAO);
  // {indicesCount, numInstancesToDraw, firstIndex, baseVertex, baseInstance}
  std::unique_ptr<GLuint[]> indirectBuffer(new GLuint[cellChunks.size() * INDIRECT_DRAW_COMMAND_ARGUMENTS]);
  GLuint dataOffset = 0;
  cellPrimitiveCount = 0;
  for (unsigned int chunkIndex = 0; chunkIndex < cellChunks.size(); chunkIndex++)
    {
      if (cellChunks[chunkIndex].isInsideFrustum(frustum, 0.0f))
        {
          ++cellPrimitiveCount;
          addIndirectBufferData(&indirectBuffer[0], dataOffset, cellChunks[chunkIndex].getNumInstances(), cellChunks[chunkIndex].getInstanceOffset());
        }
    }
  cellBuffers.bind(DIBO);
  glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(GLuint) * INDIRECT_DRAW_COMMAND_ARGUMENTS * cellPrimitiveCount, &indirectBuffer[0], GL_STATIC_DRAW);
}

void LandGenerator::addIndirectBufferData(GLuint *buffer, GLuint &dataOffset, GLuint numInstances, GLuint instanceOffset)
{
  buffer[dataOffset++] = VERTICES_PER_QUAD;
  buffer[dataOffset++] = numInstances;
  buffer[dataOffset++] = 0;
  buffer[dataOffset++] = 0;
  buffer[dataOffset++] = instanceOffset;
}
