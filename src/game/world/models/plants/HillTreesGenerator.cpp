/*
 * Copyright 2019 Ilya Malgin
 * HillTreesGenerator.cpp
 * This file is part of The Sugarpunky Chocolate Disaster project
 *
 * The Sugarpunky Chocolate Disaster project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Sugarpunky Chocolate Disaster project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * See <http://www.gnu.org/licenses/>
 *
 * Purpose: contains definitions for HillTreesGenerator class
 * @version 0.1.0
 */

#include "HillTreesGenerator.h"

/**
 * @brief load both plain and low-poly models
 */
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
  //models below are expected to have the same orientation as the surface they placed on
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
  //models below are expected to have the same orientation as the surface they placed on
  lowPolyModels.emplace_back("hillTrees/hillTree8cob/hillTree8cob.obj", true, 1);
  lowPolyModels.emplace_back("hillTrees/hillTree9cob/hillTree9cob.obj", true, 1);
  lowPolyModels.emplace_back("hillTrees/hillTree10cob/hillTree10cob.obj", true, 4);

  assert(lowPolyModels.size() == models.size());
  numSurfaceOrientedModels = 3;

  cullingOffset = FRUSTUM_CULLING_DISTANCE_OFFSET;
}

/**
 * @brief initialize models chunks and accomodate hill plants models on the world map based on input maps
 * @param hillMap map of the hills
 * @param distributionMap map filled with distribution seed values
 * @param hillsNormalMap map of the hills normals
 */
void HillTreesGenerator::setup(const map2D_f &hillMap, const map2D_i &distributionMap, const map2D_vec3 &hillsNormalMap)
{
  initializeModelChunks(hillMap);
  setupMatrices(hillMap, distributionMap, hillsNormalMap);
  initializeModelRenderChunks(hillMap);
}

/**
 * @brief calculates instance matrices for hill plants models and spreads them on world map
 * @param hillMap map of the hills
 * @param distributionMap map filled with distribution seed values
 * @param hillsNormalMap map of the hills normals
 */
void HillTreesGenerator::setupMatrices(const map2D_f &hillMap, const map2D_i &distributionMap, const map2D_vec3 &hillsNormalMap)
{
  //get empty boilerplate storage to fill during (re)allocation
  map2D_mat4 matricesStorage = substituteMatricesStorage();
  std::uniform_real_distribution<float> sizeDistribution(MIN_SCALE_TREES, MAX_SCALE_TREES);
  std::uniform_real_distribution<float> sizeDistributionRocks(MIN_SCALE_ROCKS, MAX_SCALE_ROCKS);
  std::uniform_real_distribution<float> positionDistribution(MIN_POSITION_OFFSET, MAX_POSITION_OFFSET);
  std::uniform_real_distribution<float> rotationDistribution(MIN_ROTATION_OFFSET, MAX_ROTATION_OFFSET);

  size_t numberOfModels = models.size();
  std::vector<unsigned int> instanceOffsetsVector(numberOfModels, 0);

  //used for circular indexing of a particular model/chunk, repeat counter used for models with repetitions >1
  unsigned int matrixCounter = 0, chunkCounter = 0, repeatCounter = 0;
  //the same for circular indexing of surface oriented models
  unsigned int orientedMatrixCounter = 0, orientedRepeatCounter = 0;
  for (unsigned int startY = 0; startY < WORLD_HEIGHT; startY += CHUNK_SIZE)
    {
      for (unsigned int startX = 0; startX < WORLD_WIDTH; startX += CHUNK_SIZE)
        {
          std::vector<unsigned int> numInstancesVector(numberOfModels, 0);
          //need this to be set before allocation on each subsequent chunk as it depends on the previous ones
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
                  //offset on XZ to place on a tile center
                  float translationX = -HALF_WORLD_WIDTH_F + x + 0.5f;
                  float translationZ = -HALF_WORLD_HEIGHT_F + y + 0.5f;
                  float translationY = hillMap[y][x] + HILLS_OFFSET_Y +
                                      (!indicesCrossed ? (hillMap[y+1][x+1] - hillMap[y][x]) / 2 : std::abs(hillMap[y][x+1] - hillMap[y+1][x]) / 2);

                  //firstly check whether to allocate a tree at these coordinates
                  if (slope < MAX_SURFACE_SLOPE_FOR_TREES && //hill at these coordinates is not too steep
                      (hillMap[y][x] != 0 || hillMap[y+1][x+1] != 0 || hillMap[y+1][x] != 0 || hillMap[y][x+1] != 0) && //are there hills
                      (rand() % (PLANTS_DISTRIBUTION_FREQUENCY / 2 - 1)) == 0 &&            //is there a randomizer "hit"
                      distributionMap[y][x] > (PLANTS_DISTRIBUTION_FREQUENCY / 2 - 1) &&    //is a seed value at these coordinates high enough to proceed
                      translationY > 0)
                    {
                      glm::mat4 model;
                      //additional XZ offset
                      float offsetX = positionDistribution(randomizer) * (1.0f - slope);
                      float offsetZ = positionDistribution(randomizer) * (1.0f - slope);
                      glm::vec3 translationVector(translationX + offsetX, translationY, translationZ + offsetZ);
                      model = glm::translate(model, translationVector);
                      glm::vec3 rotateVector(rotationDistribution(randomizer), 1.0f, rotationDistribution(randomizer));
                      model = glm::rotate(model, glm::radians((float)(y * WORLD_WIDTH + x * 5)), rotateVector);
                      glm::vec3 scaleVector(sizeDistribution(randomizer), sizeDistribution(randomizer), sizeDistribution(randomizer));
                      model = glm::scale(model, scaleVector);

                      size_t currentModelIndex = matrixCounter % (numberOfModels - numSurfaceOrientedModels);
                      matricesStorage[currentModelIndex].emplace_back(std::move(model));
                      ++numInstancesVector[currentModelIndex];
                      ++instanceOffsetsVector[currentModelIndex];
                      ++repeatCounter;
                      //check whether we need to choose other model for allocation
                      if (repeatCounter == models[currentModelIndex].getRepeatCount())
                        {
                          ++matrixCounter;
                          repeatCounter = 0;
                        }
                      //if we allocate a tree, no need to allocate choco-rocks at the same coordinates
                      continue;
                    }

                    //check whether surface oriented model should be allocated here
                    if (slope < MAX_SURFACE_SLOPE_FOR_ROCKS && //hill at these coordinates is not too steep
                        (hillMap[y][x] != 0 || hillMap[y+1][x+1] != 0 || hillMap[y+1][x] != 0 || hillMap[y][x+1] != 0) &&
                        (rand() % (PLANTS_DISTRIBUTION_FREQUENCY / 2 + 1)) == 0 && //is there a randomizer "hit"
                        translationY > 1.0f)
                      {
                        glm::mat4 model;
                        glm::vec3 translationVector(translationX, translationY, translationZ);
                        model = glm::translate(model, translationVector);

                        //create change of basis matrix to get same orientation as the hill surface at these coordinates
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

                        size_t surfaceOrientedModelIndex = numberOfModels - numSurfaceOrientedModels + (orientedMatrixCounter % numSurfaceOrientedModels);
                        matricesStorage[surfaceOrientedModelIndex].emplace_back(std::move(model));
                        ++numInstancesVector[surfaceOrientedModelIndex];
                        ++instanceOffsetsVector[surfaceOrientedModelIndex];
                        ++orientedRepeatCounter;
                        //check whether we need to choose other model for allocation
                        if (orientedRepeatCounter == models[surfaceOrientedModelIndex].getRepeatCount())
                          {
                            ++orientedMatrixCounter;
                            orientedRepeatCounter = 0;
                          }
                      }
                }
            }
          chunks.at(chunkCounter).setNumInstancesVector(numInstancesVector);
          ++chunkCounter;
        }
    }
  loadMatrices(matricesStorage);
}
