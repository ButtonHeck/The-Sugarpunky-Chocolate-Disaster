#include "game/world/plants/GrassGenerator.h"

GrassGenerator::GrassGenerator()
  :
    PlantGenerator()
{
  models.reserve(8);
  models.emplace_back("grass1/grass1.obj", false);
  models.emplace_back("grass2/grass2.obj", false);
  models.emplace_back("grass3/grass3.obj", false);
  models.emplace_back("grass4/grass4.obj", false);
  models.emplace_back("grass5/grass5.obj", false);
  models.emplace_back("grass6/grass6.obj", false);

  lowPolyModels.reserve(8);
  lowPolyModels.emplace_back("grass1LP/grass1LP.obj", true);
  lowPolyModels.emplace_back("grass2LP/grass2LP.obj", true);
  lowPolyModels.emplace_back("grass3LP/grass3LP.obj", true);
  lowPolyModels.emplace_back("grass4LP/grass4LP.obj", true);
  lowPolyModels.emplace_back("grass5LP/grass5LP.obj", true);
  lowPolyModels.emplace_back("grass6LP/grass6LP.obj", true);

  assert(lowPolyModels.size() == models.size());
}

void GrassGenerator::setup(const map2D_f &baseMap,
                           const map2D_f &hillMap,
                           const map2D_i &distributionMap)
{
  setupModelChunks();
  setupMatrices(baseMap, hillMap, distributionMap);
}

void GrassGenerator::setupMatrices(const map2D_f &baseMap,
                                   const map2D_f &hillMap,
                                   const map2D_i &distributionMap)
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
