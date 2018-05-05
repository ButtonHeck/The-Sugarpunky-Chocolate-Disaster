#include "TreeGenerator.h"

TreeGenerator::TreeGenerator(std::initializer_list<Model> plainModels, std::initializer_list<Model> hillModels)
  :
    plainTrees(plainModels),
    hillTrees(hillModels)
{

}

void TreeGenerator::draw(Shader &shader)
{
  for (Model& model : plainTrees)
    model.draw(shader);
  for (Model& model: hillTrees)
    model.draw(shader);
}

void TreeGenerator::setupPlainModels(std::vector<std::vector<float> > &baseMap, std::vector<std::vector<float> > &hillMap)
{
  std::vector<std::vector<glm::mat4>> treeModelsVecs;
  for (unsigned int i = 0; i < plainTrees.size(); i++)
    {
      treeModelsVecs.push_back(std::vector<glm::mat4>());
      if (!treeModels.empty())
        delete[] treeModels[i];
    }
  treeModels.clear();
  std::uniform_real_distribution<float> modelSizeDistribution(0.2f, 0.3f);
  std::uniform_real_distribution<float> modelPositionDistribution(-0.3f, 0.3f);
  std::default_random_engine randomizer;
  unsigned int treeCounter = 0;
  for (unsigned int y = 0; y < TILES_HEIGHT; y++)
    {
      for (unsigned int x = 0; x < TILES_WIDTH; x++)
        {
          if ((baseMap[y][x] == 0 && baseMap[y+1][x+1] == 0 && baseMap[y+1][x] == 0 && baseMap[y][x+1] == 0)
              && !(hillMap[y][x] != 0 || hillMap[y+1][x+1] != 0 || hillMap[y+1][x] != 0 || hillMap[y][x+1] != 0)
              && rand() % 23 == 0)
            {
              glm::mat4 model;
              model = glm::translate(model,
                                     glm::vec3(-TILES_WIDTH / 2.0f + x + modelPositionDistribution(randomizer) + 0.5f,
                                               0.0f,
                                               -TILES_HEIGHT / 2.0f + y + modelPositionDistribution(randomizer) + 0.5f));
              model = glm::rotate(model, glm::radians((float)(y * TILES_WIDTH + x * 5)), glm::vec3(0.0f, 1.0f, 0.0f));
              model = glm::scale(model, glm::vec3(modelSizeDistribution(randomizer)));
              ++treeCounter;
              treeModelsVecs[treeCounter % treeModelsVecs.size()].push_back(model);
            }
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
  std::uniform_real_distribution<float> modelSizeDistribution(0.2f, 0.3f);
  std::uniform_real_distribution<float> modelPositionDistribution(-0.2f, 0.2f);
  std::vector<std::vector<glm::mat4>> hillTreeModelsVecs;
  for (unsigned int i = 0; i < hillTrees.size(); i++)
    {
      hillTreeModelsVecs.push_back(std::vector<glm::mat4>());
      if (!hillTreeModels.empty())
        delete[] hillTreeModels[i];
    }
  hillTreeModels.clear();
  unsigned int hillTreeCounter = 0;
  for (unsigned int y = 0; y < TILES_HEIGHT; y++)
    {
      for (unsigned int x = 0; x < TILES_WIDTH; x++)
        {
          auto maxHeight = std::max(hillMap[y][x], std::max(hillMap[y][x+1], std::max(hillMap[y+1][x], hillMap[y+1][x+1])));
          auto minHeight = std::min(hillMap[y][x], std::min(hillMap[y][x+1], std::min(hillMap[y+1][x], hillMap[y+1][x+1])));
          auto slope = maxHeight - minHeight;
          if (slope < 0.8f
              && (hillMap[y][x] != 0 || hillMap[y+1][x+1] != 0 || hillMap[y+1][x] != 0 || hillMap[y][x+1] != 0)
              && rand() % 2 == 0)
            {
              bool indicesCrossed = false;
              if ((hillMap[y][x+1] > 0 && hillMap[y][x] == 0 && hillMap[y+1][x] == 0 && hillMap[y+1][x+1] == 0)
                  || (hillMap[y+1][x] > 0 && hillMap[y][x] == 0 && hillMap[y+1][x+1] == 0 && hillMap[y][x+1] == 0))
                indicesCrossed = true;
              glm::mat4 model;
              glm::vec3 translation(
                    -TILES_WIDTH / 2.0f + x + 0.5f + modelPositionDistribution(randomizer),
                    hillMap[y][x] + (!indicesCrossed ?
                      (hillMap[y+1][x+1] - hillMap[y][x]) / 2 : std::abs(hillMap[y][x+1] - hillMap[y+1][x]) / 2),
                    -TILES_HEIGHT / 2.0f + y + 0.5f + modelPositionDistribution(randomizer));
              model = glm::translate(model, translation);
              model = glm::rotate(model, glm::radians((float)(y * TILES_WIDTH + x * 5)), glm::vec3(0.0f, 1.0f, 0.0f));
              model = glm::scale(model, glm::vec3(modelSizeDistribution(randomizer)));
              ++hillTreeCounter;
              hillTreeModelsVecs[hillTreeCounter % hillTreeModelsVecs.size()].push_back(model);
            }
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

void TreeGenerator::serialize(std::ofstream &out)
{
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
