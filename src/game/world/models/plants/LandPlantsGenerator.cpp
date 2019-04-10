/*
 * Copyright 2019 Ilya Malgin
 * LandPlantsGenerator.cpp
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
 * Purpose: contains definitions for LandPlantsGenerator class
 * @version 0.1.0
 */

#include "game/world/models/plants/LandPlantsGenerator.h"

/**
 * @brief load both plain and low-poly models
 */
LandPlantsGenerator::LandPlantsGenerator()
  :
    PlantGenerator()
{
  models.reserve(16);
  models.emplace_back("landTrees/tree1/tree1.obj", false);
  models.emplace_back("landTrees/tree1_2/tree1_2.obj", false);
  models.emplace_back("landTrees/tree2/tree2.obj", false);
  models.emplace_back("landTrees/tree2_2/tree2_2.obj", false);
  models.emplace_back("landTrees/tree3/tree3.obj", false);
  models.emplace_back("landTrees/tree3_2/tree3_2.obj", false);
  models.emplace_back("landTrees/tree4/tree4.obj", false);
  models.emplace_back("landTrees/tree5/tree5.obj", false);
  models.emplace_back("landTrees/tree5_2/tree5_2.obj", false);
  models.emplace_back("landTrees/tree6/tree6.obj", false);
  models.emplace_back("landTrees/tree6_2/tree6_2.obj", false);
  models.emplace_back("landTrees/tree7/tree7.obj", false);
  models.emplace_back("landTrees/tree8/tree8.obj", false);

  lowPolyModels.reserve(16);
  lowPolyModels.emplace_back("landTrees/tree1LP/tree1LP.obj", true);
  lowPolyModels.emplace_back("landTrees/tree1_2LP/tree1_2LP.obj", true);
  lowPolyModels.emplace_back("landTrees/tree2LP/tree2LP.obj", true);
  lowPolyModels.emplace_back("landTrees/tree2_2LP/tree2_2LP.obj", true);
  lowPolyModels.emplace_back("landTrees/tree3LP/tree3LP.obj", true);
  lowPolyModels.emplace_back("landTrees/tree3_2LP/tree3_2LP.obj", true);
  lowPolyModels.emplace_back("landTrees/tree4LP/tree4LP.obj", true);
  lowPolyModels.emplace_back("landTrees/tree5LP/tree5LP.obj", true);
  lowPolyModels.emplace_back("landTrees/tree5_2LP/tree5_2LP.obj", true);
  lowPolyModels.emplace_back("landTrees/tree6LP/tree6LP.obj", true);
  lowPolyModels.emplace_back("landTrees/tree6_2LP/tree6_2LP.obj", true);
  lowPolyModels.emplace_back("landTrees/tree7LP/tree7LP.obj", true);
  lowPolyModels.emplace_back("landTrees/tree8LP/tree8LP.obj", true);

  assert(lowPolyModels.size() == models.size());

  cullingOffset = FRUSTUM_CULLING_DISTANCE_OFFSET;
}

/**
 * @brief initialize models chunks and accomodate land plants models on the world map based on input maps
 * @param landMap map of the land
 * @param hillMap map of the hills
 * @param distributionMap map filled with distribution seed values
 */
void LandPlantsGenerator::setup(const map2D_f &landMap, const map2D_f &hillMap, const map2D_i &distributionMap)
{
  initializeModelChunks(landMap);
  setupMatrices(landMap, hillMap, distributionMap);
  initializeModelRenderChunks(landMap);
}

/**
 * @brief calculates instance matrices for land plants models and spreads them on world map
 * @param landMap map of the land
 * @param hillMap map of the hills
 * @param distributionMap map filled with distribution seed values
 */
void LandPlantsGenerator::setupMatrices(const map2D_f &landMap, const map2D_f &hillMap, const map2D_i &distributionMap)
{
  //get empty boilerplate storage to fill during (re)allocation
  map2D_mat4 matricesStorage = substituteMatricesStorage();
  std::uniform_real_distribution<float> sizeDistribution(MIN_SCALE, MAX_SCALE);
  std::uniform_real_distribution<float> positionDistribution(MIN_POSITION_OFFSET, MAX_POSITION_OFFSET);

  size_t numberOfModels = models.size();
  std::vector<unsigned int> instanceOffsetsVector(numberOfModels, 0);

  //used for circular indexing of a particular model/chunk
  unsigned int matrixCounter = 0, chunkCounter = 0;
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
                  if ( (landMap[y][x] == 0 && landMap[y+1][x+1] == 0 && landMap[y+1][x] == 0 && landMap[y][x+1] == 0) && //is there land
                      !(hillMap[y][x] != 0 || hillMap[y+1][x+1] != 0 || hillMap[y+1][x] != 0 || hillMap[y][x+1] != 0) && //is there no hills
                      (rand() % (PLANTS_DISTRIBUTION_FREQUENCY / 2)) == 0 &&      //is there a randomizer "hit"
                      distributionMap[y][x] > PLANTS_DISTRIBUTION_FREQUENCY / 2)  //is a seed value at these coordinates high enough to proceed
                    {
                      glm::mat4 model;
                      //offset on XZ to place on a tile center
                      glm::vec3 translationVector(-HALF_WORLD_WIDTH_F + x + positionDistribution(randomizer) + 0.5f,
                                                  0.0f,
                                                  -HALF_WORLD_HEIGHT_F + y + positionDistribution(randomizer) + 0.5f);
                      model = glm::translate(model, translationVector);
                      model = glm::rotate(model, glm::radians((float)(y * WORLD_WIDTH + x * 5)), glm::vec3(0.0f, 1.0f, 0.0f));
                      float scaleXandZ = sizeDistribution(randomizer);
                      glm::vec3 scaleVector(scaleXandZ, sizeDistribution(randomizer), scaleXandZ); //uniform scaling for X and Z
                      model = glm::scale(model, scaleVector);

                      size_t currentModelIndex = matrixCounter % numberOfModels;
                      matricesStorage[currentModelIndex].emplace_back(std::move(model));
                      ++numInstancesVector[currentModelIndex];
                      ++instanceOffsetsVector[currentModelIndex];
                      ++matrixCounter;
                    }
                }
            }
          chunks.at(chunkCounter).setNumInstancesVector(numInstancesVector);
          ++chunkCounter;
        }
    }
  loadMatrices(matricesStorage);
}
