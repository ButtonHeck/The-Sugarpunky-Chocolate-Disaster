#include "generator/BuildableMapGenerator.h"

BuildableMapGenerator::BuildableMapGenerator(std::vector<std::vector<float> > &baseMap, std::vector<std::vector<float> > &hillMap)
  :
    MapGenerator(),
    baseMap(baseMap),
    hillMap(hillMap)
{
  glCreateBuffers(1, &modelVbo);
  glCreateVertexArrays(1, &selectedVAO);
  glCreateBuffers(1, &selectedVBO);
  glCreateBuffers(1, &selectedEBO);
}

BuildableMapGenerator::~BuildableMapGenerator()
{
  deleteGLObjects();
}

void BuildableMapGenerator::setup()
{
  for (unsigned int y = 2; y < WORLD_HEIGHT; y++)
    {
      for (unsigned int x = 0; x < WORLD_WIDTH - 1; x++)
        {
          if (baseMap[y][x] == 0
              && baseMap[y-1][x] == 0
              && baseMap[y-1][x+1] == 0
              && baseMap[y][x+1] == 0
              && hillMap[y][x] <= -HILLS_OFFSET_Y
              && hillMap[y-1][x] <= -HILLS_OFFSET_Y
              && hillMap[y-1][x+1] <= -HILLS_OFFSET_Y
              && hillMap[y][x+1] <= -HILLS_OFFSET_Y)
            {
              map[y][x] = true;
            }
        }
    }
  createTiles(true, false, map, 0);
  tiles.shrink_to_fit();
  fillBufferData();
}

void BuildableMapGenerator::fillBufferData()
{
  glBindVertexArray(vao);
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
      model = glm::translate(model, glm::vec3(- HALF_WORLD_WIDTH + tile.mapX, 0.0f, -HALF_WORLD_HEIGHT + tile.mapY));
      instanceModels[i] = model;
    }
  glBindBuffer(GL_ARRAY_BUFFER, modelVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * tiles.size(), &instanceModels[0], GL_STATIC_DRAW);
  for (unsigned int i = 0; i < 4; ++i)
    {
      glEnableVertexAttribArray(i+3);
      glVertexAttribPointer(i+3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
      glVertexAttribDivisor(i+3, 1);
    }
  delete[] instanceModels;
  resetAllGLBuffers();

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
  MapGenerator::deleteGLObjects();
  glDeleteVertexArrays(1, &selectedVAO);
  glDeleteBuffers(1, &modelVbo);
  glDeleteBuffers(1, &selectedVBO);
  glDeleteBuffers(1, &selectedEBO);
}
