#include "game/world/plants/LandPlantsGenerator.h"

LandPlantsGenerator::LandPlantsGenerator()
  :
    PlantGenerator()
{
  models.reserve(16);
  models.emplace_back("tree1/tree1.obj");
  models.emplace_back("tree1_2/tree1_2.obj");
  models.emplace_back("tree2/tree2.obj");
  models.emplace_back("tree2_2/tree2_2.obj");
  models.emplace_back("tree3/tree3.obj");
  models.emplace_back("tree3_2/tree3_2.obj");
  models.emplace_back("tree4/tree4.obj");
  models.emplace_back("tree5/tree5.obj");
  models.emplace_back("tree5_2/tree5_2.obj");
  models.emplace_back("tree6/tree6.obj");
  models.emplace_back("tree6_2/tree6_2.obj");
  models.emplace_back("tree7/tree7.obj");
  models.emplace_back("tree8/tree8.obj");

  lowPolyModels.reserve(16);
  lowPolyModels.emplace_back("tree1/tree1.obj");
  lowPolyModels.emplace_back("tree1_2/tree1_2.obj");
  lowPolyModels.emplace_back("tree2/tree2.obj");
  lowPolyModels.emplace_back("tree2_2/tree2_2.obj");
  lowPolyModels.emplace_back("tree3/tree3.obj");
  lowPolyModels.emplace_back("tree3_2/tree3_2.obj");
  lowPolyModels.emplace_back("tree4/tree4.obj");
  lowPolyModels.emplace_back("tree5/tree5.obj");
  lowPolyModels.emplace_back("tree5_2/tree5_2.obj");
  lowPolyModels.emplace_back("tree6/tree6.obj");
  lowPolyModels.emplace_back("tree6_2/tree6_2.obj");
  lowPolyModels.emplace_back("tree7/tree7.obj");
  lowPolyModels.emplace_back("tree8/tree8.obj");
}

void LandPlantsGenerator::setup(const map2D_f &baseMap,
                                const map2D_f &hillMap,
                                const map2D_i &distributionMap)
{
  setupModelChunks();
  setupMatrices(baseMap, hillMap, distributionMap);
}

void LandPlantsGenerator::setupMatrices(const map2D_f &baseMap,
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
                      && rand() % (MODELS_DISTRIBUTION_FREQ / 2) == 0
                      && distributionMap[y1][x1] > MODELS_DISTRIBUTION_FREQ / 2)
                    {
                      glm::mat4 model;
                      model = glm::translate(model,
                                             glm::vec3(-HALF_WORLD_WIDTH_F + x1 + modelPositionDistribution(randomizer) + 0.5f,
                                                       0.0f,
                                                       -HALF_WORLD_HEIGHT_F + y1 + modelPositionDistribution(randomizer) + 0.5f));
                      model = glm::rotate(model, glm::radians((float)(y1 * WORLD_WIDTH + x1 * 5)), glm::vec3(0.0f, 1.0f, 0.0f));
                      model = glm::scale(model, glm::vec3(modelSizeDistribution(randomizer)));
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
