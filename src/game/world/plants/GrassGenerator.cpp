#include "game/world/plants/GrassGenerator.h"

GrassGenerator::GrassGenerator()
  :
    PlantGenerator()
{
  models.reserve(16);
  models.emplace_back("grass1/grass1.obj");
  models.emplace_back("grass2/grass2.obj");
  models.emplace_back("grass3/grass3.obj");
  models.emplace_back("grass4/grass4.obj");
  models.emplace_back("grass5/grass5.obj");
  models.emplace_back("grass6/grass6.obj");
}

void GrassGenerator::setup(std::vector<std::vector<float> > &baseMap,
                           std::vector<std::vector<float> > &hillMap,
                           std::vector<std::vector<int> > &distributionMap)
{
  for (Model& model : models)
    {
      for (Mesh& mesh : model.getMeshes())
        mesh.setup();
    }
  setupModelChunks();
  setupMatrices(baseMap, hillMap, distributionMap);
}

void GrassGenerator::setupMatrices(std::vector<std::vector<float> > &baseMap,
                                   std::vector<std::vector<float> > &hillMap,
                                   std::vector<std::vector<int> > &distributionMap)
{
  auto matricesVecs = substituteMatricesStorage();
  std::uniform_real_distribution<float> modelSizeDistribution(0.27f, 0.32f);
  std::uniform_real_distribution<float> modelPositionDistribution(-0.25f, 0.25f);

  std::vector<unsigned int> instanceOffsetsVector(models.size());
  std::vector<unsigned int> numInstanceVector(models.size());
  for (unsigned int i = 0; i < models.size(); i++)
    {
      instanceOffsetsVector.emplace_back(0);
      numInstanceVector.emplace_back(0);
    }

  unsigned int matrixCounter = 0, chunkCounter = 0;
  for (unsigned int y = 0; y < WORLD_HEIGHT; y += CHUNK_SIZE)
    {
      for (unsigned int x = 0; x < WORLD_WIDTH; x += CHUNK_SIZE)
        {
          chunks.at(chunkCounter).setInstanceOffsetsVector(instanceOffsetsVector);
          for (unsigned int y1 = y; y1 < y + CHUNK_SIZE; y1++)
            {
              for (unsigned int x1 = x; x1 < x + CHUNK_SIZE; x1++)
                {
                  if ((baseMap[y1][x1] == 0 && baseMap[y1+1][x1+1] == 0 && baseMap[y1+1][x1] == 0 && baseMap[y1][x1+1] == 0)
                      && !(hillMap[y1][x1] != 0 || hillMap[y1+1][x1+1] != 0 || hillMap[y1+1][x1] != 0 || hillMap[y1][x1+1] != 0)
                      && rand() % (MODELS_DISTRIBUTION_FREQ / 2 - 1) == 0
                      && distributionMap[y1][x1] > MODELS_DISTRIBUTION_FREQ / 2)
                    {
                      glm::mat4 model;
                      model = glm::translate(model,
                                             glm::vec3(-HALF_WORLD_WIDTH_F + x1 + 0.5f,
                                                       0.0f,
                                                       -HALF_WORLD_HEIGHT_F + y1 + 0.5f));
                      model = glm::rotate(model, glm::radians((float)(rand() * WORLD_WIDTH + x1 * 5)), glm::vec3(0.0f, 1.0f, 0.0f));
                      glm::vec3 scale(modelSizeDistribution(randomizer) * 0.75f,
                                      modelSizeDistribution(randomizer) * 0.66f,
                                      modelSizeDistribution(randomizer) * 0.75f);
                      model = glm::scale(model, scale);
                      matricesVecs[matrixCounter % matricesVecs.size()].emplace_back(std::move(model));
                      ++numInstanceVector[matrixCounter % matricesVecs.size()];
                      ++instanceOffsetsVector[matrixCounter % matricesVecs.size()];
                      ++matrixCounter;
                    }
                }
            }
          chunks.at(chunkCounter).setNumInstancesVector(numInstanceVector);
          for (unsigned int i = 0; i < numInstanceVector.size(); i++)
            numInstanceVector[i] = 0;
          ++chunkCounter;
        }
    }
  loadMatrices(matricesVecs);
}
