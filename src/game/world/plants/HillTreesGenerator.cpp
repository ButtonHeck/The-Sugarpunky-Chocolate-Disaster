#include "game/world/plants/HillTreesGenerator.h"

HillTreesGenerator::HillTreesGenerator()
  :
    PlantGenerator()
{
  models.reserve(16);
  models.emplace_back("hillTree1/hillTree1.obj", false, 3);
  models.emplace_back("hillTree2/hillTree2.obj", false);
  models.emplace_back("hillTree3/hillTree3.obj", false, 3);
  models.emplace_back("hillTree4/hillTree4.obj", false);
  models.emplace_back("hillTree5/hillTree5.obj", false);
  models.emplace_back("hillTree6/hillTree6.obj", false);
  models.emplace_back("hillTree7/hillTree7.obj", false, 3);

  lowPolyModels.reserve(16);
  lowPolyModels.emplace_back("hillTree1LP/hillTree1LP.obj", true, 3);
  lowPolyModels.emplace_back("hillTree2LP/hillTree2LP.obj", true);
  lowPolyModels.emplace_back("hillTree3LP/hillTree3LP.obj", true, 3);
  lowPolyModels.emplace_back("hillTree4LP/hillTree4LP.obj", true);
  lowPolyModels.emplace_back("hillTree5LP/hillTree5LP.obj", true);
  lowPolyModels.emplace_back("hillTree6LP/hillTree6LP.obj", true);
  lowPolyModels.emplace_back("hillTree7LP/hillTree7LP.obj", true, 3);

  assert(lowPolyModels.size() == models.size());
}

void HillTreesGenerator::setup(const map2D_f &hillMap,
                               const map2D_i &distributionMap)
{
  setupModelChunks();
  setupMatrices(hillMap, distributionMap);
}

void HillTreesGenerator::setupMatrices(const map2D_f &hillMap,
                                       const map2D_i &distributionMap)
{
  auto matricesVecs = substituteMatricesStorage();
  std::uniform_real_distribution<float> modelSizeDistribution(0.36f, 0.51f);
  std::uniform_real_distribution<float> modelPositionDistribution(-0.6f, 0.6f);
  std::uniform_real_distribution<float> modelAxisRotationDistribution(-0.05f, 0.05f);

  std::vector<unsigned int> instanceOffsetsVector(models.size());
  std::vector<unsigned int> numInstancesVector(models.size());
  for (unsigned int i = 0; i < models.size(); i++)
    {
      instanceOffsetsVector.emplace_back(0);
      numInstancesVector.emplace_back(0);
    }

  unsigned int matrixCounter = 0, chunkCounter = 0, repeatCounter = 0;
  for (unsigned int y = 0; y < WORLD_HEIGHT; y += CHUNK_SIZE)
    {
      for (unsigned int x = 0; x < WORLD_WIDTH; x += CHUNK_SIZE)
        {
          chunks.at(chunkCounter).setInstanceOffsetsVector(instanceOffsetsVector);
          for (unsigned int y1 = y; y1 < y + CHUNK_SIZE; y1++)
            {
              for (unsigned int x1 = x; x1 < x + CHUNK_SIZE; x1++)
                {
                  auto maxHeight = std::max(hillMap[y1][x1], std::max(hillMap[y1][x1+1], std::max(hillMap[y1+1][x1], hillMap[y1+1][x1+1])));
                  auto minHeight = std::min(hillMap[y1][x1], std::min(hillMap[y1][x1+1], std::min(hillMap[y1+1][x1], hillMap[y1+1][x1+1])));
                  auto slope = maxHeight - minHeight;
                  if (slope < 1.0f
                      && (hillMap[y1][x1] != 0 || hillMap[y1+1][x1+1] != 0 || hillMap[y1+1][x1] != 0 || hillMap[y1][x1+1] != 0)
                      && rand() % (MODELS_DISTRIBUTION_FREQ / 2 - 1) == 0
                      && distributionMap[y1][x1] > (MODELS_DISTRIBUTION_FREQ / 2 - 1))
                    {
                      bool indicesCrossed = false;
                      if ((hillMap[y1][x1+1] > 0 && hillMap[y1][x1] == 0 && hillMap[y1+1][x1] == 0 && hillMap[y1+1][x1+1] == 0)
                          || (hillMap[y1+1][x1] > 0 && hillMap[y1][x1] == 0 && hillMap[y1+1][x1+1] == 0 && hillMap[y1][x1+1] == 0))
                        indicesCrossed = true;
                      glm::mat4 model;
                      float offsetX = modelPositionDistribution(randomizer) * (1.0f - slope);
                      float offsetZ = modelPositionDistribution(randomizer) * (1.0f - slope);
                      float baseY = hillMap[y1][x1] + (!indicesCrossed ?
                             (hillMap[y1+1][x1+1] - hillMap[y1][x1]) / 2 :
                              std::abs(hillMap[y1][x1+1] - hillMap[y1+1][x1]) / 2)
                            + HILLS_OFFSET_Y;
                      glm::vec3 translation(
                            -HALF_WORLD_WIDTH_F + x1 + 0.5f + offsetX,
                            baseY,
                            -HALF_WORLD_HEIGHT_F + y1 + 0.5f + offsetZ);
                      if (translation.y < 0)
                        continue;
                      model = glm::translate(model, translation);
                      model = glm::rotate(model, glm::radians((float)(y1 * WORLD_WIDTH + x1 * 5)),
                                          glm::vec3(modelAxisRotationDistribution(randomizer), 1.0f, modelAxisRotationDistribution(randomizer)));
                      model = glm::scale(model, glm::vec3(modelSizeDistribution(randomizer)));

                      size_t modelIndex = matrixCounter % matricesVecs.size();
                      matricesVecs[modelIndex].emplace_back(std::move(model));
                      ++numInstancesVector[modelIndex];
                      ++instanceOffsetsVector[modelIndex];
                      ++repeatCounter;
                      if (repeatCounter == models[modelIndex].getRepeatCount())
                        {
                          ++matrixCounter;
                          repeatCounter = 0;
                        }
                    }
                }
            }
          chunks.at(chunkCounter).setNumInstancesVector(numInstancesVector);
          for (unsigned int i = 0; i < numInstancesVector.size(); i++)
            numInstancesVector[i] = 0;
          ++chunkCounter;
        }
    }
  loadMatrices(matricesVecs);
}
