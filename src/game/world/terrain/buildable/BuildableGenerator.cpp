#include "BuildableGenerator.h"

BuildableGenerator::BuildableGenerator()
  :
    Generator(),
    selectedBuffers(VAO | VBO | EBO)
{
  //this collection already have VAO/VBO/EBO in Generator ctor
  basicGLBuffers.add(INSTANCE_VBO);
}

void BuildableGenerator::setup(const map2D_f &landMap, const map2D_f &hillsMap)
{
  for (unsigned int y = UPPER_LEFT_CORNER_START_Y; y < WORLD_HEIGHT; y++)
    {
      for (unsigned int x = UPPER_LEFT_CORNER_START_X; x < WORLD_WIDTH - 1; x++)
        {
          map[y][x] = false;
          if (landMap[y][x] == 0 &&
              landMap[y-1][x] == 0 &&
              landMap[y-1][x+1] == 0 &&
              landMap[y][x+1] == 0 &&
              hillsMap[y][x] <= -HILLS_OFFSET_Y &&
              hillsMap[y-1][x] <= -HILLS_OFFSET_Y &&
              hillsMap[y-1][x+1] <= -HILLS_OFFSET_Y &&
              hillsMap[y][x+1] <= -HILLS_OFFSET_Y)
            {
              map[y][x] = true;
            }
        }
    }
  createTiles(true, false, map, 0);
  tiles.shrink_to_fit();
  fillBufferData();
}

void BuildableGenerator::fillBufferData()
{
  setupAndBindBuffers(selectedBuffers);
  BufferCollection::bindZero(VAO | VBO | EBO);

  setupAndBindBuffers(basicGLBuffers);
  std::unique_ptr<glm::vec4[]> instancesTranslations(new glm::vec4[tiles.size()]);
  for (unsigned int tileIndex = 0; tileIndex < tiles.size(); tileIndex++)
    {
      TerrainTile& tile = tiles[tileIndex];
      instancesTranslations[tileIndex] = glm::vec4(-HALF_WORLD_WIDTH + tile.mapX, 0.0f, -HALF_WORLD_HEIGHT + tile.mapY, 0.0f);
    }
  basicGLBuffers.bind(INSTANCE_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * tiles.size(), &instancesTranslations[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
  glVertexAttribDivisor(1, 1);

  BufferCollection::bindZero(VAO | VBO | EBO);
}

void BuildableGenerator::setupAndBindBuffers(BufferCollection &buffers)
{
  constexpr static GLfloat CELL_VERTICES[12] = {
       0.05f, 0.01f,  -0.05f,
       0.95f, 0.01f,  -0.05f,
       0.95f, 0.01f,  -0.95f,
       0.05f, 0.01f,  -0.95f
  };
  buffers.bind(VAO | VBO | EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER, sizeof(CELL_VERTICES), CELL_VERTICES, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
}
