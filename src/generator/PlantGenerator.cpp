#include "generator/PlantGenerator.h"

PlantGenerator::PlantGenerator(int numGrassModels)
  :
    numGrassModels(numGrassModels)
{
  randomizer.seed(std::chrono::system_clock::now().time_since_epoch().count());
  plainPlants.emplace_back("tree1/tree1.obj");
  plainPlants.emplace_back("tree1_2/tree1_2.obj");
  plainPlants.emplace_back("tree2/tree2.obj");
  plainPlants.emplace_back("tree2_2/tree2_2.obj");
  plainPlants.emplace_back("tree3/tree3.obj");
  plainPlants.emplace_back("tree3_2/tree3_2.obj");
  plainPlants.emplace_back("tree4/tree4.obj");
  plainPlants.emplace_back("tree5/tree5.obj");
  plainPlants.emplace_back("tree5_2/tree5_2.obj");
  plainPlants.emplace_back("tree6/tree6.obj");
  plainPlants.emplace_back("tree6_2/tree6_2.obj");
  plainPlants.emplace_back("tree7/tree7.obj");
  plainPlants.emplace_back("tree8/tree8.obj");
  plainPlants.emplace_back("grass1/grass1.obj");
  plainPlants.emplace_back("grass2/grass2.obj");
  plainPlants.emplace_back("grass3/grass3.obj");
  plainPlants.emplace_back("grass4/grass4.obj");
  plainPlants.emplace_back("grass5/grass5.obj");
  plainPlants.emplace_back("grass6/grass6.obj");
  hillTrees.emplace_back("hillTree1/hillTree1.obj");
  hillTrees.emplace_back("hillTree2/hillTree2.obj");
  hillTrees.emplace_back("hillTree3/hillTree3.obj");
  hillTrees.emplace_back("hillTree4/hillTree4.obj");
  hillTrees.emplace_back("hillTree5/hillTree5.obj");
  hillTrees.emplace_back("hillTree6/hillTree6.obj");
  hillTrees.emplace_back("hillTree7/hillTree7.obj");
  hillTrees.emplace_back("hillTree1/hillTree1.obj");
  hillTrees.emplace_back("hillTree3/hillTree3.obj");
  hillTrees.emplace_back("hillTree7/hillTree7.obj");
  hillTrees.emplace_back("hillTree1/hillTree1.obj");
  hillTrees.emplace_back("hillTree3/hillTree3.obj");
  hillTrees.emplace_back("hillTree7/hillTree7.obj");
}

PlantGenerator::~PlantGenerator()
{
  delete[] numPlainPlants;
  delete[] numHillTrees;
  int treeModelsVecSize = plainPlantsMatrices.size();
  for (int i = 0; i < treeModelsVecSize; ++i)
    {
      glm::mat4* models = plainPlantsMatrices[i];
      delete[] models;
      models = nullptr;
    }
  int hillTreeModelsVecSize = hillTreesMatrices.size();
  for (int i = 0; i < hillTreeModelsVecSize; ++i)
    {
      glm::mat4* models = hillTreesMatrices[i];
      delete[] models;
      models = nullptr;
    }
}

void PlantGenerator::setupPlainModels(std::vector<std::vector<float> > &baseMap, std::vector<std::vector<float> > &hillMap)
{
  plainPlantsModelChunks.clear();
  for (unsigned int y = 0; y < WORLD_HEIGHT; y += CHUNK_SIZE)
    {
      for (unsigned int x = 0; x < WORLD_WIDTH; x += CHUNK_SIZE)
        {
          ModelChunk chunk(x, x + CHUNK_SIZE, y, y + CHUNK_SIZE);
          plainPlantsModelChunks.emplace_back(std::move(chunk));
        }
    }
  std::vector<std::vector<glm::mat4>> treeModelsVecs;
  for (unsigned int i = 0; i < plainPlants.size(); i++)
    {
      treeModelsVecs.emplace_back(std::vector<glm::mat4>());
      if (!plainPlantsMatrices.empty())
        delete[] plainPlantsMatrices[i];
    }
  plainPlantsMatrices.clear();
  std::uniform_real_distribution<float> modelSizeDistribution(0.27f, 0.32f);
  std::uniform_real_distribution<float> modelPositionDistribution(-0.25f, 0.25f);
  unsigned int treeCounter = 0, chunkCounter = 0;

  std::vector<unsigned int> instanceOffsetsVector(plainPlants.size());
  std::vector<unsigned int> numInstanceVector(plainPlants.size());
  for (unsigned int i = 0; i < plainPlants.size(); i++)
    {
      instanceOffsetsVector.emplace_back(0);
      numInstanceVector.emplace_back(0);
    }

  for (unsigned int y = 0; y < WORLD_HEIGHT; y += CHUNK_SIZE)
    {
      for (unsigned int x = 0; x < WORLD_WIDTH; x += CHUNK_SIZE)
        {
          plainPlantsModelChunks.at(chunkCounter).setInstanceOffsetsVector(instanceOffsetsVector);
          for (unsigned int y1 = y; y1 < y + CHUNK_SIZE; y1++)
            {
              for (unsigned int x1 = x; x1 < x + CHUNK_SIZE; x1++)
                {
                  if ((baseMap[y1][x1] == 0 && baseMap[y1+1][x1+1] == 0 && baseMap[y1+1][x1] == 0 && baseMap[y1][x1+1] == 0)
                      && !(hillMap[y1][x1] != 0 || hillMap[y1+1][x1+1] != 0 || hillMap[y1+1][x1] != 0 || hillMap[y1][x1+1] != 0)
                      && rand() % 8 == 0)
                    {
                      glm::mat4 model;
                      model = glm::translate(model,
                                             glm::vec3(-(float)HALF_WORLD_WIDTH + x1 + modelPositionDistribution(randomizer) + 0.5f,
                                                       0.0f,
                                                       -(float)HALF_WORLD_HEIGHT + y1 + modelPositionDistribution(randomizer) + 0.5f));
                      model = glm::rotate(model, glm::radians((float)(y1 * WORLD_WIDTH + x1 * 5)), glm::vec3(0.0f, 1.0f, 0.0f));
                      model = glm::scale(model, glm::vec3(modelSizeDistribution(randomizer), modelSizeDistribution(randomizer), modelSizeDistribution(randomizer)));
                      treeModelsVecs[treeCounter % (treeModelsVecs.size() - numGrassModels)].emplace_back(std::move(model));
                      numInstanceVector[treeCounter % (treeModelsVecs.size() - numGrassModels)] += 1;
                      instanceOffsetsVector[treeCounter % (treeModelsVecs.size() - numGrassModels)] += 1;
                      ++treeCounter;
                    }
                }
            }
          int grassCounter = 0;
          for (unsigned int y1 = y; y1 < y + CHUNK_SIZE; y1++)
            {
              for (unsigned int x1 = x; x1 < x + CHUNK_SIZE; x1++)
                {
                  if ((baseMap[y1][x1] == 0 && baseMap[y1+1][x1+1] == 0 && baseMap[y1+1][x1] == 0 && baseMap[y1][x1+1] == 0)
                      && !(hillMap[y1][x1] != 0 || hillMap[y1+1][x1+1] != 0 || hillMap[y1+1][x1] != 0 || hillMap[y1][x1+1] != 0)
                      && rand() % 6 == 0)
                    {
                      glm::mat4 model;
                      model = glm::translate(model,
                                             glm::vec3(-(float)HALF_WORLD_WIDTH + x1 + 0.5f,
                                                       0.0f,
                                                       -(float)HALF_WORLD_HEIGHT + y1 + 0.5f));
                      model = glm::rotate(model, glm::radians((float)(rand() * WORLD_WIDTH + x1 * 5)), glm::vec3(0.0f, 1.0f, 0.0f));
                      glm::vec3 scale(modelSizeDistribution(randomizer) * 0.75f,
                                      modelSizeDistribution(randomizer) * 0.66f,
                                      modelSizeDistribution(randomizer) * 0.75f);
                      model = glm::scale(model, scale);
                      treeModelsVecs[treeModelsVecs.size() - numGrassModels + (grassCounter % numGrassModels)].emplace_back(std::move(model));
                      numInstanceVector[treeModelsVecs.size() - numGrassModels + (grassCounter % numGrassModels)] += 1;
                      instanceOffsetsVector[treeModelsVecs.size() - numGrassModels + (grassCounter % numGrassModels)] += 1;
                      ++grassCounter;
                    }
                }
            }
          plainPlantsModelChunks.at(chunkCounter).setNumInstancesVector(numInstanceVector);
          for (unsigned int i = 0; i < numInstanceVector.size(); i++)
            {
              numInstanceVector[i] = 0;
            }
          ++chunkCounter;
        }
    }
  if (plainPlantsAlreadyCreated)
    delete[] numPlainPlants;
  numPlainPlants = new unsigned int[plainPlants.size()];
  for (unsigned int i = 0; i < treeModelsVecs.size(); i++)
    {
      plainPlantsMatrices.emplace_back(new glm::mat4[treeModelsVecs[i].size()]);
      for (unsigned int m = 0; m < treeModelsVecs[i].size(); m++)
        {
          plainPlantsMatrices[i][m] = treeModelsVecs[i][m];
        }
      numPlainPlants[i] = treeModelsVecs[i].size();
    }
  for (unsigned int i = 0; i < plainPlants.size(); i++)
    {
      plainPlants[i].loadInstances(plainPlantsMatrices[i], numPlainPlants[i]);
    }
  plainPlantsAlreadyCreated = true;
}

void PlantGenerator::updatePlainModels(std::vector<glm::mat4 *> &models, unsigned int *numAllTrees)
{
  delete[] numPlainPlants;
  numPlainPlants = new unsigned int[plainPlants.size()];
  for (unsigned int i = 0; i < plainPlantsMatrices.size(); i++)
    {
      delete[] plainPlantsMatrices[i];
    }
  plainPlantsMatrices.clear();
  for (unsigned int i = 0; i < models.size(); i++)
    {
      plainPlantsMatrices.emplace_back(new glm::mat4[numAllTrees[i]]);
      numPlainPlants[i] = numAllTrees[i];
    }
  for (unsigned int i = 0; i < plainPlantsMatrices.size(); i++)
    {
      for (unsigned int m = 0; m < numAllTrees[i]; m++)
        {
          plainPlantsMatrices[i][m] = models[i][m];
        }
    }
  for (unsigned int i = 0; i < plainPlants.size(); i++)
    {
      plainPlants[i].loadInstances(plainPlantsMatrices[i], numPlainPlants[i]);
    }
}

void PlantGenerator::setupHillModels(std::vector<std::vector<float> > &hillMap)
{
  hillTreesModelChunks.clear();
  for (unsigned int y = 0; y < WORLD_HEIGHT; y += CHUNK_SIZE)
    {
      for (unsigned int x = 0; x < WORLD_WIDTH; x += CHUNK_SIZE)
        {
          ModelChunk chunk(x, x + CHUNK_SIZE, y, y + CHUNK_SIZE);
          hillTreesModelChunks.emplace_back(std::move(chunk));
        }
    }
  std::uniform_real_distribution<float> modelSizeDistribution(0.36f, 0.51f);
  std::uniform_real_distribution<float> modelPositionDistribution(-0.6f, 0.6f);
  std::uniform_real_distribution<float> modelAxisRotationDistribution(-0.05f, 0.05f);
  std::vector<std::vector<glm::mat4>> hillTreeModelsVecs;
  for (unsigned int i = 0; i < hillTrees.size(); i++)
    {
      hillTreeModelsVecs.emplace_back(std::vector<glm::mat4>());
      if (!hillTreesMatrices.empty())
        delete[] hillTreesMatrices[i];
    }
  hillTreesMatrices.clear();
  unsigned int hillTreeCounter = 0, chunkCounter = 0;

  std::vector<unsigned int> instanceOffsetsVector(hillTrees.size());
  std::vector<unsigned int> numInstancesVector(hillTrees.size());
  for (unsigned int i = 0; i < hillTrees.size(); i++)
    {
      instanceOffsetsVector.emplace_back(0);
      numInstancesVector.emplace_back(0);
    }

  for (unsigned int y = 0; y < WORLD_HEIGHT; y += CHUNK_SIZE)
    {
      for (unsigned int x = 0; x < WORLD_WIDTH; x += CHUNK_SIZE)
        {
          hillTreesModelChunks.at(chunkCounter).setInstanceOffsetsVector(instanceOffsetsVector);
          for (unsigned int y1 = y; y1 < y + CHUNK_SIZE; y1++)
            {
              for (unsigned int x1 = x; x1 < x + CHUNK_SIZE; x1++)
                {
                  auto maxHeight = std::max(hillMap[y1][x1], std::max(hillMap[y1][x1+1], std::max(hillMap[y1+1][x1], hillMap[y1+1][x1+1])));
                  auto minHeight = std::min(hillMap[y1][x1], std::min(hillMap[y1][x1+1], std::min(hillMap[y1+1][x1], hillMap[y1+1][x1+1])));
                  auto slope = maxHeight - minHeight;
                  if (slope < 1.0f
                      && (hillMap[y1][x1] != 0 || hillMap[y1+1][x1+1] != 0 || hillMap[y1+1][x1] != 0 || hillMap[y1][x1+1] != 0)
                      && rand() % 3 == 0)
                    {
                      bool indicesCrossed = false;
                      if ((hillMap[y1][x1+1] > 0 && hillMap[y1][x1] == 0 && hillMap[y1+1][x1] == 0 && hillMap[y1+1][x1+1] == 0)
                          || (hillMap[y1+1][x1] > 0 && hillMap[y1][x1] == 0 && hillMap[y1+1][x1+1] == 0 && hillMap[y1][x1+1] == 0))
                        indicesCrossed = true;
                      glm::mat4 model;
                      float offsetX = modelPositionDistribution(randomizer) * (1.0f - slope / 1.0f);
                      float offsetZ = modelPositionDistribution(randomizer) * (1.0f - slope / 1.0f);
                      float baseY = hillMap[y1][x1] + (!indicesCrossed ?
                             (hillMap[y1+1][x1+1] - hillMap[y1][x1]) / 2 :
                              std::abs(hillMap[y1][x1+1] - hillMap[y1+1][x1]) / 2)
                            + HILLS_OFFSET_Y;
                      glm::vec3 translation(
                            -(float)HALF_WORLD_WIDTH + x1 + 0.5f + offsetX,
                            baseY,
                            -(float)HALF_WORLD_HEIGHT + y1 + 0.5f + offsetZ);
                      if (translation.y < 0)
                        continue;
                      model = glm::translate(model, translation);
                      model = glm::rotate(model, glm::radians((float)(y1 * WORLD_WIDTH + x1 * 5)),
                                          glm::vec3(modelAxisRotationDistribution(randomizer), 1.0f, modelAxisRotationDistribution(randomizer)));
                      model = glm::scale(model, glm::vec3(modelSizeDistribution(randomizer)));
                      hillTreeModelsVecs[hillTreeCounter % hillTreeModelsVecs.size()].emplace_back(std::move(model));
                      numInstancesVector[hillTreeCounter % hillTreeModelsVecs.size()] += 1;
                      instanceOffsetsVector[hillTreeCounter % hillTreeModelsVecs.size()] += 1;
                      ++hillTreeCounter;
                    }
                }
            }
          hillTreesModelChunks.at(chunkCounter).setNumInstancesVector(numInstancesVector);
          for (unsigned int i = 0; i < numInstancesVector.size(); i++)
            {
              numInstancesVector[i] = 0;
            }
          ++chunkCounter;
        }
    }
  if (hillTreesAlreadyCreated)
    delete[] numHillTrees;
  numHillTrees = new unsigned int[hillTrees.size()];
  for (unsigned int i = 0; i < hillTreeModelsVecs.size(); i++)
    {
      hillTreesMatrices.emplace_back(new glm::mat4[hillTreeModelsVecs[i].size()]);
      for (unsigned int m = 0; m < hillTreeModelsVecs[i].size(); m++)
        {
          hillTreesMatrices[i][m] = hillTreeModelsVecs[i][m];
        }
      numHillTrees[i] = hillTreeModelsVecs[i].size();
    }
  for (unsigned int i = 0; i < hillTrees.size(); i++)
    {
      hillTrees[i].loadInstances(hillTreesMatrices[i], numHillTrees[i]);
    }
  hillTreesAlreadyCreated = true;
}

void PlantGenerator::updateHillModels(std::vector<glm::mat4 *> &models, unsigned int *numAllTrees)
{
  delete[] numHillTrees;
  numHillTrees = new unsigned int[hillTrees.size()];
  for (unsigned int i = 0; i < hillTreesMatrices.size(); i++)
    {
      delete[] hillTreesMatrices[i];
    }
  hillTreesMatrices.clear();
  for (unsigned int i = 0; i < models.size(); i++)
    {
      hillTreesMatrices.emplace_back(new glm::mat4[numAllTrees[i]]);
      numHillTrees[i] = numAllTrees[i];
    }
  for (unsigned int i = 0; i < hillTreesMatrices.size(); i++)
    {
      for (unsigned int m = 0; m < numAllTrees[i]; m++)
        {
          hillTreesMatrices[i][m] = models[i][m];
        }
    }
  for (unsigned int i = 0; i < hillTrees.size(); i++)
    {
      hillTrees[i].loadInstances(hillTreesMatrices[i], numHillTrees[i]);
    }
}

std::vector<glm::mat4 *> &PlantGenerator::getPlainPlantsMatrices()
{
  return plainPlantsMatrices;
}

std::vector<glm::mat4 *> &PlantGenerator::getHillTreesMatrices()
{
  return hillTreesMatrices;
}

unsigned int PlantGenerator::getNumPlainPlants(int i)
{
  return numPlainPlants[i];
}

unsigned int PlantGenerator::getNumHillTrees(int i)
{
  return numHillTrees[i];
}

std::vector<Model> &PlantGenerator::getPlainPlants()
{
  return plainPlants;
}

std::vector<Model> &PlantGenerator::getHillTrees()
{
  return hillTrees;
}

std::vector<ModelChunk> &PlantGenerator::getPlainPlantsModelChunks()
{
  return plainPlantsModelChunks;
}

std::vector<ModelChunk> &PlantGenerator::getHillTreeModelChunks()
{
  return hillTreesModelChunks;
}

void PlantGenerator::serialize(std::ofstream &output)
{
  for (unsigned int chunk = 0; chunk < plainPlantsModelChunks.size(); chunk++)
    {
      for (unsigned int i = 0; i < plainPlantsModelChunks[chunk].getNumInstancesVector().size(); i++)
        {
          output << plainPlantsModelChunks[chunk].getNumInstances(i) << " ";
        }
      for (unsigned int i = 0; i < plainPlantsModelChunks[chunk].getInstanceOffsetVector().size(); i++)
        {
          output << plainPlantsModelChunks[chunk].getInstanceOffset(i) << " ";
        }
    }
  for (unsigned int chunk = 0; chunk < hillTreesModelChunks.size(); chunk++)
    {
      for (unsigned int i = 0; i < hillTreesModelChunks[chunk].getNumInstancesVector().size(); i++)
        {
          output << hillTreesModelChunks[chunk].getNumInstances(i) << " ";
        }
      for (unsigned int i = 0; i < hillTreesModelChunks[chunk].getInstanceOffsetVector().size(); i++)
        {
          output << hillTreesModelChunks[chunk].getInstanceOffset(i) << " ";
        }
    }
  for (unsigned int i = 0; i < plainPlantsMatrices.size(); i++)
    {
      output << numPlainPlants[i] << " ";
      for (unsigned int m = 0; m < numPlainPlants[i]; m++)
        {
          float* matrixValues = (float*)glm::value_ptr(plainPlantsMatrices[i][m]);
          for (unsigned int e = 0; e < 16; ++e)
            output << matrixValues[e] << " ";
        }
    }
  for (unsigned int i = 0; i < hillTreesMatrices.size(); i++)
    {
      output << numHillTrees[i] << " ";
      for (unsigned int m = 0; m < numHillTrees[i]; m++)
        {
          float* matrixValues = (float*)glm::value_ptr(hillTreesMatrices[i][m]);
          for (unsigned int e = 0; e < 16; ++e)
            output << matrixValues[e] << " ";
        }
    }
}

void PlantGenerator::deserialize(std::ifstream &input)
{
  for (unsigned int chunk = 0; chunk < plainPlantsModelChunks.size(); chunk++)
    {
      for (unsigned int i = 0; i < plainPlantsModelChunks[chunk].getNumInstancesVector().size(); i++)
        {
          unsigned int numInstances;
          input >> numInstances;
          plainPlantsModelChunks[chunk].setNumInstances(i, numInstances);
        }
      for (unsigned int i = 0; i < plainPlantsModelChunks[chunk].getInstanceOffsetVector().size(); i++)
        {
          unsigned int offset;
          input >> offset;
          plainPlantsModelChunks[chunk].setInstanceOffset(i, offset);
        }
    }
  for (unsigned int chunk = 0; chunk < hillTreesModelChunks.size(); chunk++)
    {
      for (unsigned int i = 0; i < hillTreesModelChunks[chunk].getNumInstancesVector().size(); i++)
        {
          unsigned int numInstances;
          input >> numInstances;
          hillTreesModelChunks[chunk].setNumInstances(i, numInstances);
        }
      for (unsigned int i = 0; i < hillTreesModelChunks[chunk].getInstanceOffsetVector().size(); i++)
        {
          unsigned int offset;
          input >> offset;
          hillTreesModelChunks[chunk].setInstanceOffset(i, offset);
        }
    }

  std::vector<glm::mat4*> newPlainPlantsMatrices;
  unsigned int numAllTrees[plainPlantsMatrices.size()];
  for (unsigned int i = 0; i < plainPlantsMatrices.size(); i++)
    {
      unsigned int numTrees = 0;
      input >> numTrees;
      numAllTrees[i] = numTrees;
      newPlainPlantsMatrices.emplace_back(new glm::mat4[numTrees]);
      for (unsigned int t = 0; t < numTrees; t++)
        {
          glm::mat4 model;
          float* modelData = (float*)glm::value_ptr(model);
          for (unsigned int e = 0; e < 16; e++)
            {
              input >> modelData[e];
            }
          newPlainPlantsMatrices[i][t] = std::move(model);
        }
    }
  std::vector<glm::mat4*> newHillTreesMatrices;
  unsigned int numAllHillTrees[hillTreesMatrices.size()];
  for (unsigned int i = 0; i < hillTreesMatrices.size(); i++)
    {
      unsigned int numHillTrees = 0;
      input >> numHillTrees;
      numAllHillTrees[i] = numHillTrees;
      newHillTreesMatrices.emplace_back(new glm::mat4[numHillTrees]);
      for (unsigned int m = 0; m < numHillTrees; m++)
        {
          glm::mat4 model;
          float* modelData = (float*)glm::value_ptr(model);
          for (unsigned int e = 0; e < 16; e++)
            {
              input >> modelData[e];
            }
          newHillTreesMatrices[i][m] = std::move(model);
        }
    }
  updatePlainModels(newPlainPlantsMatrices, numAllTrees);
  updateHillModels(newHillTreesMatrices, numAllHillTrees);
  for (unsigned int i = 0; i < getPlainPlantsMatrices().size(); i++)
    {
      delete[] newPlainPlantsMatrices[i];
    }
  for (unsigned int i = 0; i < getHillTreesMatrices().size(); i++)
    {
      delete[] newHillTreesMatrices[i];
    }
}
