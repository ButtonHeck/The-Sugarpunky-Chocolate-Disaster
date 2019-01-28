#include "game/world/models/plants/HillTreesGenerator.h"

HillTreesGenerator::HillTreesGenerator()
  :
    PlantGenerator()
{
  models.reserve(16);
  models.emplace_back("hillTrees/hillTree1/hillTree1.obj", false, 3);
  models.emplace_back("hillTrees/hillTree2/hillTree2.obj", false);
  models.emplace_back("hillTrees/hillTree3/hillTree3.obj", false, 3);
  models.emplace_back("hillTrees/hillTree4/hillTree4.obj", false);
  models.emplace_back("hillTrees/hillTree5/hillTree5.obj", false);
  models.emplace_back("hillTrees/hillTree6/hillTree6.obj", false);
  models.emplace_back("hillTrees/hillTree7/hillTree7.obj", false, 3);
  models.emplace_back("hillTrees/hillTree8cob/hillTree8cob.obj", false, 1, true);
  models.emplace_back("hillTrees/hillTree9cob/hillTree9cob.obj", false, 1, true);
  models.emplace_back("hillTrees/hillTree10cob/hillTree10cob.obj", false, 4, true);

  lowPolyModels.reserve(16);
  lowPolyModels.emplace_back("hillTrees/hillTree1LP/hillTree1LP.obj", true, 3);
  lowPolyModels.emplace_back("hillTrees/hillTree2LP/hillTree2LP.obj", true);
  lowPolyModels.emplace_back("hillTrees/hillTree3LP/hillTree3LP.obj", true, 3);
  lowPolyModels.emplace_back("hillTrees/hillTree4LP/hillTree4LP.obj", true);
  lowPolyModels.emplace_back("hillTrees/hillTree5LP/hillTree5LP.obj", true);
  lowPolyModels.emplace_back("hillTrees/hillTree6LP/hillTree6LP.obj", true);
  lowPolyModels.emplace_back("hillTrees/hillTree7LP/hillTree7LP.obj", true, 3);
  lowPolyModels.emplace_back("hillTrees/hillTree8cob/hillTree8cob.obj", true, 1, true);
  lowPolyModels.emplace_back("hillTrees/hillTree9cob/hillTree9cob.obj", true, 1, true);
  lowPolyModels.emplace_back("hillTrees/hillTree10cob/hillTree10cob.obj", true, 4, true);

  assert(lowPolyModels.size() == models.size());
  numSurfaceModels = 3;
}

void HillTreesGenerator::setup(const map2D_f &hillMap,
                               const map2D_i &distributionMap,
                               const map2D_vec3 &hillsNormalMap)
{
  setupModelChunks();
  setupMatrices(hillMap, distributionMap, hillsNormalMap);
}

void HillTreesGenerator::setupMatrices(const map2D_f &hillMap,
                                       const map2D_i &distributionMap,
                                       const map2D_vec3 &hillsNormalMap)
{
  auto matricesVecs = substituteMatricesStorage();
  std::uniform_real_distribution<float> sizeDistribution(0.36f, 0.51f);
  std::uniform_real_distribution<float> positionDistribution(-0.6f, 0.6f);
  std::uniform_real_distribution<float> rotationDistribution(-0.05f, 0.05f);

  std::vector<unsigned int> instanceOffsetsVector(models.size());
  std::vector<unsigned int> numInstancesVector(models.size());
  for (unsigned int i = 0; i < models.size(); i++)
    {
      instanceOffsetsVector.emplace_back(0);
      numInstancesVector.emplace_back(0);
    }

  unsigned int matrixCounter = 0, chunkCounter = 0, repeatCounter = 0, surfaceMatrixCounter = 0, surfaceRepeatCounter = 0;
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
                  bool indicesCrossed = false;
                  if ((hillMap[y1][x1+1] > 0 && hillMap[y1][x1] == 0 && hillMap[y1+1][x1] == 0 && hillMap[y1+1][x1+1] == 0)
                      || (hillMap[y1+1][x1] > 0 && hillMap[y1][x1] == 0 && hillMap[y1+1][x1+1] == 0 && hillMap[y1][x1+1] == 0))
                    indicesCrossed = true;
                  float translationX = -HALF_WORLD_WIDTH_F + x1 + 0.5f;
                  float translationZ = -HALF_WORLD_HEIGHT_F + y1 + 0.5f;
                  float translationY = hillMap[y1][x1] + (!indicesCrossed ?
                         (hillMap[y1+1][x1+1] - hillMap[y1][x1]) / 2 :
                          std::abs(hillMap[y1][x1+1] - hillMap[y1+1][x1]) / 2)
                        + HILLS_OFFSET_Y;

                  if (slope < 1.0f
                      && (hillMap[y1][x1] != 0 || hillMap[y1+1][x1+1] != 0 || hillMap[y1+1][x1] != 0 || hillMap[y1][x1+1] != 0)
                      && rand() % (MODELS_DISTRIBUTION_FREQ / 2 - 1) == 0
                      && distributionMap[y1][x1] > (MODELS_DISTRIBUTION_FREQ / 2 - 1))
                    {
                      glm::mat4 model;
                      float offsetX = positionDistribution(randomizer) * (1.0f - slope);
                      float offsetZ = positionDistribution(randomizer) * (1.0f - slope);
                      glm::vec3 translation(translationX + offsetX, translationY, translationZ + offsetZ);
                      if (translation.y < 0)
                        continue;
                      model = glm::translate(model, translation);
                      model = glm::rotate(model, glm::radians((float)(y1 * WORLD_WIDTH + x1 * 5)),
                                          glm::vec3(rotationDistribution(randomizer), 1.0f, rotationDistribution(randomizer)));
                      model = glm::scale(model, glm::vec3(sizeDistribution(randomizer)));

                      size_t modelIndex = matrixCounter % (matricesVecs.size() - numSurfaceModels);
                      matricesVecs[modelIndex].emplace_back(std::move(model));
                      ++numInstancesVector[modelIndex];
                      ++instanceOffsetsVector[modelIndex];
                      ++repeatCounter;
                      if (repeatCounter == models[modelIndex].getRepeatCount())
                        {
                          ++matrixCounter;
                          repeatCounter = 0;
                        }
                      continue;
                    }

                    if (slope < 2.6f
                        && (hillMap[y1][x1] != 0 || hillMap[y1+1][x1+1] != 0 || hillMap[y1+1][x1] != 0 || hillMap[y1][x1+1] != 0)
                        && rand() % (MODELS_DISTRIBUTION_FREQ / 2 + 1) == 0)
                      {
                        glm::mat4 surfaceModel;
                        glm::vec3 translation(translationX, translationY, translationZ);
                        if (translation.y < 1.0f)
                          continue;
                        surfaceModel = glm::translate(surfaceModel, translation);
                        glm::vec3 hillTileNormalApprox = hillsNormalMap[y1][x1];
                        hillTileNormalApprox += hillsNormalMap[y1+1][x1];
                        hillTileNormalApprox += hillsNormalMap[y1+1][x1+1];
                        hillTileNormalApprox += hillsNormalMap[y1][x1+1];
                        glm::vec3 newY = glm::normalize(hillTileNormalApprox);
                        glm::vec3 newZ = glm::normalize(glm::cross(newY, glm::vec3(0.0f, 1.0f, 0.0f)));
                        glm::vec3 newX = glm::normalize(glm::cross(newY, newZ));
                        glm::mat4 changeOfBasisTransform = glm::mat4(glm::mat3(newX, newY, newZ));
                        surfaceModel *= changeOfBasisTransform;
                        surfaceModel = glm::rotate(surfaceModel, glm::radians((float)(y1 * WORLD_WIDTH + x1 * 29)), glm::vec3(0.0f, 1.0f, 0.0f));
                        surfaceModel = glm::scale(surfaceModel, glm::vec3(sizeDistribution(randomizer)) * 0.8f);

                        size_t surfaceModelIndex = matricesVecs.size() - numSurfaceModels + (surfaceMatrixCounter % numSurfaceModels);
                        matricesVecs[surfaceModelIndex].emplace_back(std::move(surfaceModel));
                        ++numInstancesVector[surfaceModelIndex];
                        ++instanceOffsetsVector[surfaceModelIndex];
                        ++surfaceRepeatCounter;
                        if (surfaceRepeatCounter == models[surfaceModelIndex].getRepeatCount())
                          {
                            ++surfaceMatrixCounter;
                            surfaceRepeatCounter = 0;
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
