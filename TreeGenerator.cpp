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
    treeModelsVecs.push_back(std::vector<glm::mat4>());
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
              && rand() % 31 == 0)
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
  std::vector<glm::mat4*> treeModels;
  for (unsigned int i = 0; i < treeModelsVecs.size(); i++)
    {
      treeModels.push_back(new glm::mat4[treeModelsVecs[i].size()]);
      for (unsigned int m = 0; m < treeModelsVecs[i].size(); m++)
        {
          treeModels[i][m] = treeModelsVecs[i][m];
        }
    }
  for (unsigned int i = 0; i < plainTrees.size(); i++)
    {
      plainTrees[i].loadInstances(treeModels[i], treeModelsVecs[i].size());
    }
}

void TreeGenerator::setupHillModels(std::vector<std::vector<float> > &hillMap)
{
  std::uniform_real_distribution<float> modelSizeDistribution(0.2f, 0.3f);
  std::uniform_real_distribution<float> modelPositionDistribution(-0.2f, 0.2f);
  std::vector<std::vector<glm::mat4>> hillTreeModelsVecs;
  for (unsigned int i = 0; i < hillTrees.size(); i++)
    hillTreeModelsVecs.push_back(std::vector<glm::mat4>());
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
  std::vector<glm::mat4*> hillTreeModels;
  for (unsigned int i = 0; i < hillTreeModelsVecs.size(); i++)
    {
      hillTreeModels.push_back(new glm::mat4[hillTreeModelsVecs[i].size()]);
      for (unsigned int m = 0; m < hillTreeModelsVecs[i].size(); m++)
        {
          hillTreeModels[i][m] = hillTreeModelsVecs[i][m];
        }
    }
  for (unsigned int i = 0; i < hillTrees.size(); i++)
    {
      hillTrees[i].loadInstances(hillTreeModels[i], hillTreeModelsVecs[i].size());
    }
}
