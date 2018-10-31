#include "game/world/terrain/buildable/BuildableGenerator.h"

BuildableGenerator::BuildableGenerator()
  :
    Generator(),
    selectedBuffers(VAO | VBO | EBO)
{
  basicGLBuffers.add(INSTANCE_VBO);
}

void BuildableGenerator::setup(map2D_f &landMap, map2D_f &hillsMap)
{
  for (unsigned int y = 2; y < WORLD_HEIGHT; y++)
    {
      for (unsigned int x = 0; x < WORLD_WIDTH - 1; x++)
        {
          map[y][x] = false;
          if (landMap[y][x] == 0
              && landMap[y-1][x] == 0
              && landMap[y-1][x+1] == 0
              && landMap[y][x+1] == 0
              && hillsMap[y][x] <= -HILLS_OFFSET_Y
              && hillsMap[y-1][x] <= -HILLS_OFFSET_Y
              && hillsMap[y-1][x+1] <= -HILLS_OFFSET_Y
              && hillsMap[y][x+1] <= -HILLS_OFFSET_Y)
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
  resetAllGLBuffers();

  setupAndBindBuffers(basicGLBuffers);
  numInstances = tiles.size();
  std::unique_ptr<glm::mat4[]> instanceModels(new glm::mat4[numInstances]);
  for (unsigned int i = 0; i < tiles.size(); i++)
    {
      glm::mat4 model;
      TerrainTile& tile = tiles[i];
      model = glm::translate(model, glm::vec3(-HALF_WORLD_WIDTH + tile.mapX, 0.0f, -HALF_WORLD_HEIGHT + tile.mapY));
      instanceModels[i] = model;
    }
  basicGLBuffers.bind(INSTANCE_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * tiles.size(), &instanceModels[0], GL_STATIC_DRAW);
  for (unsigned int i = 0; i < 4; ++i)
    {
      glEnableVertexAttribArray(i+3);
      glVertexAttribPointer(i+3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
      glVertexAttribDivisor(i+3, 1);
    }
  resetAllGLBuffers();
}

void BuildableGenerator::setupAndBindBuffers(OpenglBuffer &buffers)
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
