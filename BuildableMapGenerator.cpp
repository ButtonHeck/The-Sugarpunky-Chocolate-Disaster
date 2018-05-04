#include "BuildableMapGenerator.h"

BuildableMapGenerator::BuildableMapGenerator(std::vector<std::vector<float> > &baseMap, std::vector<std::vector<float> > &hillMap)
  :
    MapGenerator(),
    baseMap(baseMap),
    hillMap(hillMap)
{

}

void BuildableMapGenerator::prepareMap()
{
  for (unsigned int y = 2; y < TILES_HEIGHT; y++)
    {
      for (unsigned int x = 0; x < TILES_WIDTH - 1; x++)
        {
          if (baseMap[y][x] == 0
              && baseMap[y-1][x] == 0
              && baseMap[y-1][x+1] == 0
              && baseMap[y][x+1] == 0
              && hillMap[y][x] == 0
              && hillMap[y-1][x] == 0
              && hillMap[y-1][x+1] == 0
              && hillMap[y][x+1] == 0)
            {
              map[y][x] = true;
            }
        }
    }
  createTiles(true, false, map);
  tiles.shrink_to_fit();
}

void BuildableMapGenerator::fillBufferData()
{
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glGenBuffers(1, &modelVbo);
  GLfloat cellVertices[12] = {
       0.05f, 0.01f,  -0.05f,
       0.95f, 0.01f,  -0.05f,
       0.95f, 0.01f,  -0.95f,
       0.05f, 0.01f,  -0.95f
  };
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cellVertices), cellVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  num_instances = tiles.size();
  glm::mat4* instanceModels = new glm::mat4[num_instances];
  for (unsigned int i = 0; i < tiles.size(); i++)
    {
      glm::mat4 model;
      TerrainTile& tile = tiles[i];
      model = glm::translate(model, glm::vec3(-TILES_WIDTH / 2 + tile.mapX, 0.0f, -TILES_HEIGHT / 2 + tile.mapY));
      instanceModels[i] = model;
    }
  glBindBuffer(GL_ARRAY_BUFFER, modelVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * tiles.size(), &instanceModels[0], GL_STATIC_DRAW);
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
  delete[] instanceModels;
  resetAllGLBuffers();
  glGenVertexArrays(1, &selectedVAO);
  glGenBuffers(1, &selectedVBO);
  glGenBuffers(1, &selectedEBO);
  glBindVertexArray(selectedVAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, selectedEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, selectedVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cellVertices), cellVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  resetAllGLBuffers();
}

GLuint &BuildableMapGenerator::getNumInstances()
{
  return num_instances;
}

GLuint &BuildableMapGenerator::getSelectedTileVAO()
{
  return selectedVAO;
}

void BuildableMapGenerator::deleteGLObjects()
{
  glDeleteVertexArrays(1, &selectedVAO);
  glDeleteBuffers(1, &modelVbo);
  glDeleteBuffers(1, &selectedVBO);
  glDeleteBuffers(1, &selectedEBO);
}
