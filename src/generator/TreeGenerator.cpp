#include "src/generator/TreeGenerator.h"

TreeGenerator::TreeGenerator(std::initializer_list<Model> plainTrees, std::initializer_list<Model> hillTrees)
  :
    plainTrees(plainTrees),
    hillTrees(hillTrees)
{
}

void TreeGenerator::setupPlainModels(std::vector<std::vector<float> > &baseMap, std::vector<std::vector<float> > &hillMap)
{
  treeModelChunks.clear();
  for (unsigned int y = 0; y < TILES_HEIGHT; y += CHUNK_SIZE)
    {
      for (unsigned int x = 0; x < TILES_WIDTH; x += CHUNK_SIZE)
        {
          ModelChunk chunk(x, x + CHUNK_SIZE, y, y + CHUNK_SIZE);
          treeModelChunks.push_back(std::move(chunk));
        }
    }
  std::vector<std::vector<glm::mat4>> treeModelsVecs;
  for (unsigned int i = 0; i < plainTrees.size(); i++)
    {
      treeModelsVecs.push_back(std::vector<glm::mat4>());
      if (!treeModels.empty())
        delete[] treeModels[i];
    }
  treeModels.clear();
  std::uniform_real_distribution<float> modelSizeDistribution(0.25f, 0.3f);
  std::uniform_real_distribution<float> modelPositionDistribution(-0.3f, 0.3f);
  std::default_random_engine randomizer;
  unsigned int treeCounter = 0, chunkCounter = 0;

  std::vector<unsigned int> instanceOffsetsVector(plainTrees.size());
  std::vector<unsigned int> numInstanceVector(plainTrees.size());
  for (unsigned int i = 0; i < plainTrees.size(); i++)
    {
      instanceOffsetsVector.push_back(0);
      numInstanceVector.push_back(0);
    }

  for (unsigned int y = 0; y < TILES_HEIGHT; y += CHUNK_SIZE)
    {
      for (unsigned int x = 0; x < TILES_WIDTH; x += CHUNK_SIZE)
        {
          treeModelChunks.at(chunkCounter).setInstanceOffsetsVector(instanceOffsetsVector);
          for (unsigned int y1 = y; y1 < y + CHUNK_SIZE; y1++)
            {
              for (unsigned int x1 = x; x1 < x + CHUNK_SIZE; x1++)
                {
                  if ((baseMap[y1][x1] == 0 && baseMap[y1+1][x1+1] == 0 && baseMap[y1+1][x1] == 0 && baseMap[y1][x1+1] == 0)
                      && !(hillMap[y1][x1] != 0 || hillMap[y1+1][x1+1] != 0 || hillMap[y1+1][x1] != 0 || hillMap[y1][x1+1] != 0)
                      && rand() % 7 == 0)
                    {
                      glm::mat4 model;
                      model = glm::translate(model,
                                             glm::vec3(-(float)HALF_TILES_WIDTH + x1 + modelPositionDistribution(randomizer) + 0.5f,
                                                       0.0f,
                                                       -(float)HALF_TILES_HEIGHT + y1 + modelPositionDistribution(randomizer) + 0.5f));
                      model = glm::rotate(model, glm::radians((float)(y1 * TILES_WIDTH + x1 * 5)), glm::vec3(0.0f, 1.0f, 0.0f));
                      model = glm::scale(model, glm::vec3(modelSizeDistribution(randomizer)));
                      treeModelsVecs[treeCounter % treeModelsVecs.size()].push_back(model);
                      numInstanceVector[treeCounter % treeModelsVecs.size()] += 1;
                      instanceOffsetsVector[treeCounter % treeModelsVecs.size()] += 1;
                      ++treeCounter;
                    }
                }
            }
          treeModelChunks.at(chunkCounter).setNumInstancesVector(numInstanceVector);
          for (unsigned int i = 0; i < numInstanceVector.size(); i++)
            {
              numInstanceVector[i] = 0;
            }
          ++chunkCounter;
        }
    }
  if (treesAlreadyCreated)
    delete[] numTrees;
  numTrees = new unsigned int[plainTrees.size()];
  for (unsigned int i = 0; i < treeModelsVecs.size(); i++)
    {
      treeModels.push_back(new glm::mat4[treeModelsVecs[i].size()]);
      for (unsigned int m = 0; m < treeModelsVecs[i].size(); m++)
        {
          treeModels[i][m] = treeModelsVecs[i][m];
        }
      numTrees[i] = treeModelsVecs[i].size();
    }
  for (unsigned int i = 0; i < plainTrees.size(); i++)
    {
      plainTrees[i].loadInstances(treeModels[i], numTrees[i]);
    }
  treesAlreadyCreated = true;
}

void TreeGenerator::updatePlainModels(std::vector<glm::mat4 *> &models, unsigned int *numAllTrees)
{
  delete[] numTrees;
  numTrees = new unsigned int[plainTrees.size()];
  for (unsigned int i = 0; i < treeModels.size(); i++)
    {
      delete[] treeModels[i];
    }
  treeModels.clear();
  for (unsigned int i = 0; i < models.size(); i++)
    {
      treeModels.push_back(new glm::mat4[numAllTrees[i]]);
      numTrees[i] = numAllTrees[i];
    }
  for (unsigned int i = 0; i < treeModels.size(); i++)
    {
      for (unsigned int m = 0; m < numAllTrees[i]; m++)
        {
          treeModels[i][m] = models[i][m];
        }
    }
  for (unsigned int i = 0; i < plainTrees.size(); i++)
    {
      plainTrees[i].loadInstances(treeModels[i], numTrees[i]);
    }
}

void TreeGenerator::setupHillModels(std::vector<std::vector<float> > &hillMap)
{
  hillTreeModelChunks.clear();
  for (unsigned int y = 0; y < TILES_HEIGHT; y += CHUNK_SIZE)
    {
      for (unsigned int x = 0; x < TILES_WIDTH; x += CHUNK_SIZE)
        {
          ModelChunk chunk(x, x + CHUNK_SIZE, y, y + CHUNK_SIZE);
          hillTreeModelChunks.push_back(std::move(chunk));
        }
    }
  std::uniform_real_distribution<float> modelSizeDistribution(0.25f, 0.32f);
  std::uniform_real_distribution<float> modelPositionDistribution(-0.45f, 0.45f);
  std::vector<std::vector<glm::mat4>> hillTreeModelsVecs;
  for (unsigned int i = 0; i < hillTrees.size(); i++)
    {
      hillTreeModelsVecs.push_back(std::vector<glm::mat4>());
      if (!hillTreeModels.empty())
        delete[] hillTreeModels[i];
    }
  hillTreeModels.clear();
  unsigned int hillTreeCounter = 0, chunkCounter = 0;

  std::vector<unsigned int> instanceOffsetsVector(hillTrees.size());
  std::vector<unsigned int> numInstancesVector(hillTrees.size());
  for (unsigned int i = 0; i < hillTrees.size(); i++)
    {
      instanceOffsetsVector.push_back(0);
      numInstancesVector.push_back(0);
    }

  for (unsigned int y = 0; y < TILES_HEIGHT; y += CHUNK_SIZE)
    {
      for (unsigned int x = 0; x < TILES_WIDTH; x += CHUNK_SIZE)
        {
          hillTreeModelChunks.at(chunkCounter).setInstanceOffsetsVector(instanceOffsetsVector);
          for (unsigned int y1 = y; y1 < y + CHUNK_SIZE; y1++)
            {
              for (unsigned int x1 = x; x1 < x + CHUNK_SIZE; x1++)
                {
                  auto maxHeight = std::max(hillMap[y1][x1], std::max(hillMap[y1][x1+1], std::max(hillMap[y1+1][x1], hillMap[y1+1][x1+1])));
                  auto minHeight = std::min(hillMap[y1][x1], std::min(hillMap[y1][x1+1], std::min(hillMap[y1+1][x1], hillMap[y1+1][x1+1])));
                  auto slope = maxHeight - minHeight;
                  if (slope < 0.75f
                      && (hillMap[y1][x1] != 0 || hillMap[y1+1][x1+1] != 0 || hillMap[y1+1][x1] != 0 || hillMap[y1][x1+1] != 0)
                      && rand() % 2 == 0)
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
                            -(float)HALF_TILES_WIDTH + x1 + 0.5f + offsetX,
                            baseY,
                            -(float)HALF_TILES_HEIGHT + y1 + 0.5f + offsetZ);
                      if (translation.y < 0)
                        continue;
                      model = glm::translate(model, translation);
                      model = glm::rotate(model, glm::radians((float)(y1 * TILES_WIDTH + x1 * 5)), glm::vec3(0.0f, 1.0f, 0.0f));
                      model = glm::scale(model, glm::vec3(modelSizeDistribution(randomizer)));
                      hillTreeModelsVecs[hillTreeCounter % hillTreeModelsVecs.size()].push_back(model);
                      numInstancesVector[hillTreeCounter % hillTreeModelsVecs.size()] += 1;
                      instanceOffsetsVector[hillTreeCounter % hillTreeModelsVecs.size()] += 1;
                      ++hillTreeCounter;
                    }
                }
            }
          hillTreeModelChunks.at(chunkCounter).setNumInstancesVector(numInstancesVector);
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
      hillTreeModels.push_back(new glm::mat4[hillTreeModelsVecs[i].size()]);
      for (unsigned int m = 0; m < hillTreeModelsVecs[i].size(); m++)
        {
          hillTreeModels[i][m] = hillTreeModelsVecs[i][m];
        }
      numHillTrees[i] = hillTreeModelsVecs[i].size();
    }
  for (unsigned int i = 0; i < hillTrees.size(); i++)
    {
      hillTrees[i].loadInstances(hillTreeModels[i], numHillTrees[i]);
    }
  hillTreesAlreadyCreated = true;
}

void TreeGenerator::updateHillModels(std::vector<glm::mat4 *> &models, unsigned int *numAllTrees)
{
  delete[] numHillTrees;
  numHillTrees = new unsigned int[hillTrees.size()];
  for (unsigned int i = 0; i < hillTreeModels.size(); i++)
    {
      delete[] hillTreeModels[i];
    }
  hillTreeModels.clear();
  for (unsigned int i = 0; i < models.size(); i++)
    {
      hillTreeModels.push_back(new glm::mat4[numAllTrees[i]]);
      numHillTrees[i] = numAllTrees[i];
    }
  for (unsigned int i = 0; i < hillTreeModels.size(); i++)
    {
      for (unsigned int m = 0; m < numAllTrees[i]; m++)
        {
          hillTreeModels[i][m] = models[i][m];
        }
    }
  for (unsigned int i = 0; i < hillTrees.size(); i++)
    {
      hillTrees[i].loadInstances(hillTreeModels[i], numHillTrees[i]);
    }
}

std::vector<glm::mat4 *> &TreeGenerator::getTreeModels()
{
  return treeModels;
}

std::vector<glm::mat4 *> &TreeGenerator::getHillTreeModels()
{
  return hillTreeModels;
}

unsigned int TreeGenerator::getNumTrees(int i)
{
  return numTrees[i];
}

unsigned int TreeGenerator::getNumHillTrees(int i)
{
  return numHillTrees[i];
}

std::vector<Model> &TreeGenerator::getPlainTrees()
{
  return plainTrees;
}

std::vector<Model> &TreeGenerator::getHillTrees()
{
  return hillTrees;
}

std::vector<ModelChunk> &TreeGenerator::getTreeModelChunks()
{
  return treeModelChunks;
}

std::vector<ModelChunk> &TreeGenerator::getHillTreeModelChunks()
{
  return hillTreeModelChunks;
}

void TreeGenerator::serialize(std::ofstream &out)
{
  for (unsigned int chunk = 0; chunk < treeModelChunks.size(); chunk++)
    {
      for (unsigned int i = 0; i < treeModelChunks[chunk].getNumInstancesVector().size(); i++)
        {
          out << treeModelChunks[chunk].getNumInstances(i) << " ";
        }
      for (unsigned int i = 0; i < treeModelChunks[chunk].getInstanceOffsetVector().size(); i++)
        {
          out << treeModelChunks[chunk].getInstanceOffset(i) << " ";
        }
    }
  for (unsigned int chunk = 0; chunk < hillTreeModelChunks.size(); chunk++)
    {
      for (unsigned int i = 0; i < hillTreeModelChunks[chunk].getNumInstancesVector().size(); i++)
        {
          out << hillTreeModelChunks[chunk].getNumInstances(i) << " ";
        }
      for (unsigned int i = 0; i < hillTreeModelChunks[chunk].getInstanceOffsetVector().size(); i++)
        {
          out << hillTreeModelChunks[chunk].getInstanceOffset(i) << " ";
        }
    }
  for (unsigned int i = 0; i < treeModels.size(); i++)
    {
      out << numTrees[i] << " ";
      for (unsigned int m = 0; m < numTrees[i]; m++)
        {
          float* matrixValues = (float*)glm::value_ptr(treeModels[i][m]);
          for (unsigned int e = 0; e < 16; ++e)
            out << matrixValues[e] << " ";
        }
    }
  for (unsigned int i = 0; i < hillTreeModels.size(); i++)
    {
      out << numHillTrees[i] << " ";
      for (unsigned int m = 0; m < numHillTrees[i]; m++)
        {
          float* matrixValues = (float*)glm::value_ptr(hillTreeModels[i][m]);
          for (unsigned int e = 0; e < 16; ++e)
            out << matrixValues[e] << " ";
        }
    }
}