#include "GrassGenerator.h"

GrassGenerator::GrassGenerator(BaseMapGenerator &baseMapGenerator, HillsMapGenerator &hillsMapGenerator)
  :
    baseMapGenerator(baseMapGenerator),
    hillsMapGenerator(hillsMapGenerator)
{
}

void GrassGenerator::prepareMap()
{
  auto& baseMap = baseMapGenerator.getMap();
  auto& hillMap = hillsMapGenerator.getMap();
  for (unsigned int y = 0; y < TILES_HEIGHT; y++)
    {
      for (unsigned int x = 0; x < TILES_WIDTH; x++)
        {
          if ((baseMap[y][x] == 0 && baseMap[y+1][x+1] == 0 && baseMap[y+1][x] == 0 && baseMap[y][x+1] == 0)
              && !(hillMap[y][x] != 0 || hillMap[y+1][x+1] != 0 || hillMap[y+1][x] != 0 || hillMap[y][x+1] != 0))
            numTiles += GRASS_PER_TILE;
        }
    }
  num_instances_per_vao = numTiles / (NUM_VAOS - 1);
  for (unsigned int i = 0; i < NUM_VAOS; i++)
    instanceModels.push_back(new glm::mat4[num_instances_per_vao]);
  std::cout << "num grass tiles: " << numTiles << std::endl;
  std::cout << "num tiles per grass VAO: " << num_instances_per_vao << std::endl;
  unsigned int modelCounter = 0;
  std::uniform_real_distribution<float> grassDistribution(0, 1.0f / GRASS_PER_TILE);
  std::uniform_real_distribution<float> grassScaleDistribution(0.1f, 0.13f);
  for (unsigned int y = 0; y < TILES_HEIGHT; y++)
    {
      for (unsigned int x = 0; x < TILES_WIDTH; x++)
        {
          if ((baseMap[y][x] == 0 && baseMap[y+1][x+1] == 0 && baseMap[y+1][x] == 0 && baseMap[y][x+1] == 0)
              && !(hillMap[y][x] != 0 || hillMap[y+1][x+1] != 0 || hillMap[y+1][x] != 0 || hillMap[y][x+1] != 0))
            {
              for (unsigned int n = 0; n < GRASS_PER_TILE; n++)
                {
                  glm::mat4 model;
                  model = glm::translate(model,
                                         glm::vec3(-TILES_WIDTH / 2.0f + x + grassDistribution(randomizer) * (n+1),
                                                   0.0f,
                                                   -TILES_HEIGHT / 2.0f + y + grassDistribution(randomizer) * (n+1)));
                  model = glm::scale(model, glm::vec3(grassScaleDistribution(randomizer) * 2));
                  model = glm::rotate(model, glm::radians((float)modelCounter * (n+1)), glm::vec3(0.0f, 1.0f, 0.0f));
                  instanceModels.at(modelCounter / num_instances_per_vao)[modelCounter % num_instances_per_vao] = model;
                  modelCounter++;
                }
            }
        }
    }
}

void GrassGenerator::fillBufferData()
{
  GLfloat vertices[20] = {
    0.0f, 0.0f,  0.0f, 0.0f,  0.0f,
    1.0f, 0.0f,  0.0f, 1.0f,  0.0f,
    1.0f, 1.0f,  0.0f, 1.0f,  1.0f,
    0.0f, 1.0f,  0.0f, 0.0f,  1.0f
  };
  GLuint modelVbos[NUM_VAOS];
  for (unsigned int i = 0; i < NUM_VAOS; i++)
    glGenVertexArrays(1, &modelVaos[i]);
  glGenBuffers(NUM_VAOS, modelVbos);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  for (unsigned int i = 0; i < NUM_VAOS; i++)
    {
      glBindVertexArray(modelVaos[i]);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
      glBindBuffer(GL_ARRAY_BUFFER, modelVbos[i]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * num_instances_per_vao, &instanceModels[i][0], GL_STATIC_DRAW);
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
      delete[] instanceModels.at(i);
    }
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint &GrassGenerator::getVAO(int i)
{
  return modelVaos[i];
}

unsigned int &GrassGenerator::getNumVAOS()
{
  return NUM_VAOS;
}

unsigned int &GrassGenerator::getNumInstancesPerVao()
{
  return num_instances_per_vao;
}

void GrassGenerator::deleteGLObjects()
{
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteVertexArrays(NUM_VAOS, modelVaos);
}
