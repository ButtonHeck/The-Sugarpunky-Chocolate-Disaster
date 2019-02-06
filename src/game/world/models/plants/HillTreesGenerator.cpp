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
  //models below are expected to has the same orientation as the surface the land on
  models.emplace_back("hillTrees/hillTree8cob/hillTree8cob.obj", false, 1);
  models.emplace_back("hillTrees/hillTree9cob/hillTree9cob.obj", false, 1);
  models.emplace_back("hillTrees/hillTree10cob/hillTree10cob.obj", false, 4);

  lowPolyModels.reserve(16);
  lowPolyModels.emplace_back("hillTrees/hillTree1LP/hillTree1LP.obj", true, 3);
  lowPolyModels.emplace_back("hillTrees/hillTree2LP/hillTree2LP.obj", true);
  lowPolyModels.emplace_back("hillTrees/hillTree3LP/hillTree3LP.obj", true, 3);
  lowPolyModels.emplace_back("hillTrees/hillTree4LP/hillTree4LP.obj", true);
  lowPolyModels.emplace_back("hillTrees/hillTree5LP/hillTree5LP.obj", true);
  lowPolyModels.emplace_back("hillTrees/hillTree6LP/hillTree6LP.obj", true);
  lowPolyModels.emplace_back("hillTrees/hillTree7LP/hillTree7LP.obj", true, 3);
  //models below are expected to has the same orientation as the surface the land on
  lowPolyModels.emplace_back("hillTrees/hillTree8cob/hillTree8cob.obj", true, 1);
  lowPolyModels.emplace_back("hillTrees/hillTree9cob/hillTree9cob.obj", true, 1);
  lowPolyModels.emplace_back("hillTrees/hillTree10cob/hillTree10cob.obj", true, 4);

  assert(lowPolyModels.size() == models.size());
  numSurfaceOrientedModels = 3;
}

void HillTreesGenerator::setup(const map2D_f &hillMap, const map2D_i &distributionMap, const map2D_vec3 &hillsNormalMap)
{
  setupModelChunks();
  setupMatrices(hillMap, distributionMap, hillsNormalMap);
}

void HillTreesGenerator::setupMatrices(const map2D_f &hillMap, const map2D_i &distributionMap, const map2D_vec3 &hillsNormalMap)
{
  map2D_mat4 matricesStorage = substituteMatricesStorage();
  std::uniform_real_distribution<float> sizeDistribution(MIN_SCALE_TREES, MAX_SCALE_TREES);
  std::uniform_real_distribution<float> sizeDistributionRocks(MIN_SCALE_ROCKS, MAX_SCALE_ROCKS);
  std::uniform_real_distribution<float> positionDistribution(MIN_POSITION_OFFSET, MAX_POSITION_OFFSET);
  std::uniform_real_distribution<float> rotationDistribution(MIN_ROTATION_OFFSET, MAX_ROTATION_OFFSET);

  size_t numberOfModels = models.size();
  std::vector<unsigned int> instanceOffsetsVector(numberOfModels);
  std::vector<unsigned int> numInstancesVector(numberOfModels);
  for (unsigned int i = 0; i < numberOfModels; i++)
    {
      instanceOffsetsVector.emplace_back(0);
      numInstancesVector.emplace_back(0);
    }

  unsigned int matrixCounter = 0, chunkCounter = 0, repeatCounter = 0, surfaceMatrixCounter = 0, surfaceRepeatCounter = 0;
  for (unsigned int startY = 0; startY < WORLD_HEIGHT; startY += CHUNK_SIZE)
    {
      for (unsigned int startX = 0; startX < WORLD_WIDTH; startX += CHUNK_SIZE)
        {
          chunks.at(chunkCounter).setInstanceOffsetsVector(instanceOffsetsVector);
          for (unsigned int y = startY; y < startY + CHUNK_SIZE; y++)
            {
              for (unsigned int x = startX; x < startX + CHUNK_SIZE; x++)
                {
                  float maxHeight = std::max(hillMap[y][x], std::max(hillMap[y][x+1], std::max(hillMap[y+1][x], hillMap[y+1][x+1])));
                  float minHeight = std::min(hillMap[y][x], std::min(hillMap[y][x+1], std::min(hillMap[y+1][x], hillMap[y+1][x+1])));
                  float slope = maxHeight - minHeight;
                  bool indicesCrossed = false;
                  if ((hillMap[y][x+1] > 0 && hillMap[y][x] == 0 && hillMap[y+1][x] == 0 && hillMap[y+1][x+1] == 0) ||
                      (hillMap[y+1][x] > 0 && hillMap[y][x] == 0 && hillMap[y][x+1] == 0 && hillMap[y+1][x+1] == 0))
                    indicesCrossed = true;
                  float translationX = -HALF_WORLD_WIDTH_F + x + 0.5f;
                  float translationZ = -HALF_WORLD_HEIGHT_F + y + 0.5f;
                  float translationY = hillMap[y][x] + HILLS_OFFSET_Y +
                                      (!indicesCrossed ? (hillMap[y+1][x+1] - hillMap[y][x]) / 2 : std::abs(hillMap[y][x+1] - hillMap[y+1][x]) / 2);

                  if (slope < MAX_SURFACE_SLOPE_FOR_TREES &&
                      (hillMap[y][x] != 0 || hillMap[y+1][x+1] != 0 || hillMap[y+1][x] != 0 || hillMap[y][x+1] != 0) &&
                      (rand() % (PLANTS_DISTRIBUTION_FREQUENCY / 2 - 1)) == 0 &&
                      distributionMap[y][x] > (PLANTS_DISTRIBUTION_FREQUENCY / 2 - 1) &&
                      translationY > 0)
                    {
                      glm::mat4 model;
                      float offsetX = positionDistribution(randomizer) * (1.0f - slope);
                      float offsetZ = positionDistribution(randomizer) * (1.0f - slope);
                      glm::vec3 translationVector(translationX + offsetX, translationY, translationZ + offsetZ);
                      model = glm::translate(model, translationVector);
                      glm::vec3 rotateVector(rotationDistribution(randomizer), 1.0f, rotationDistribution(randomizer));
                      model = glm::rotate(model, glm::radians((float)(y * WORLD_WIDTH + x * 5)), rotateVector);
                      glm::vec3 scaleVector(sizeDistribution(randomizer), sizeDistribution(randomizer), sizeDistribution(randomizer));
                      model = glm::scale(model, scaleVector);

                      size_t currentModelIndex = matrixCounter % (matricesStorage.size() - numSurfaceOrientedModels);
                      matricesStorage[currentModelIndex].emplace_back(std::move(model));
                      ++numInstancesVector[currentModelIndex];
                      ++instanceOffsetsVector[currentModelIndex];
                      ++repeatCounter;
                      if (repeatCounter == models[currentModelIndex].getRepeatCount())
                        {
                          ++matrixCounter;
                          repeatCounter = 0;
                        }
                      continue;
                    }

                    if (slope < MAX_SURFACE_SLOPE_FOR_ROCKS &&
                        (hillMap[y][x] != 0 || hillMap[y+1][x+1] != 0 || hillMap[y+1][x] != 0 || hillMap[y][x+1] != 0) &&
                        (rand() % (PLANTS_DISTRIBUTION_FREQUENCY / 2 + 1)) == 0 &&
                        translationY > 1.0f)
                      {
                        glm::mat4 model;
                        glm::vec3 translationVector(translationX, translationY, translationZ);
                        model = glm::translate(model, translationVector);

                        //create change of basis matrix to get same orientation as the surface on these coordinates
                        glm::vec3 hillTileNormalApprox = hillsNormalMap[y][x] +
                                                         hillsNormalMap[y+1][x] +
                                                         hillsNormalMap[y+1][x+1] +
                                                         hillsNormalMap[y][x+1];
                        glm::vec3 newY = glm::normalize(hillTileNormalApprox);
                        glm::vec3 newZ = glm::normalize(glm::cross(newY, glm::vec3(0.0f, 1.0f, 0.0f)));
                        glm::vec3 newX = glm::normalize(glm::cross(newY, newZ));
                        glm::mat4 changeOfBasisTransform = glm::mat4(glm::mat3(newX, newY, newZ));
                        model *= changeOfBasisTransform;

                        model = glm::rotate(model, glm::radians((float)(y * WORLD_WIDTH + x * 29)), glm::vec3(0.0f, 1.0f, 0.0f));
                        glm::vec3 scaleVector(sizeDistributionRocks(randomizer)); //uniform scaling
                        model = glm::scale(model, scaleVector);

                        size_t surfaceModelIndex = matricesStorage.size() - numSurfaceOrientedModels + (surfaceMatrixCounter % numSurfaceOrientedModels);
                        matricesStorage[surfaceModelIndex].emplace_back(std::move(model));
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
  loadMatrices(matricesStorage);
}
