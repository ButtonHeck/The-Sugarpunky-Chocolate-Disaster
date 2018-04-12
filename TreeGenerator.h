#ifndef TREEGENERATOR_H
#define TREEGENERATOR_H
#include <vector>
#include <fstream>
#include "Model.h"
#include "Shader.h"
#include "Settings.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

class TreeGenerator
{
public:
  TreeGenerator(std::initializer_list<Model> plainModels, std::initializer_list<Model> hillModels);
  void draw(Shader& shader);
  void setupPlainModels(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap);
  void setupHillModels(std::vector<std::vector<float>>& hillMap);
  std::vector<glm::mat4*>& getTreeModels();
  std::vector<glm::mat4*>& getHillTreeModels();
  unsigned int getNumTrees(int i);
  unsigned int getNumHillTrees(int i);
  std::vector<Model>& getPlainTrees();
  std::vector<Model>& getHillTrees();
  void serialize(std::ofstream& out);
private:
  std::vector<Model> plainTrees;
  std::vector<glm::mat4*> treeModels;
  unsigned int* numTrees;
  std::vector<Model> hillTrees;
  std::vector<glm::mat4*> hillTreeModels;
  unsigned int* numHillTrees;
  std::default_random_engine randomizer;
  bool treesAlreadyCreated = false;
  bool hillTreesAlreadyCreated = false;
};

#endif // TREEGENERATOR_H
