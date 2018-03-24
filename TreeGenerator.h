#ifndef TREEGENERATOR_H
#define TREEGENERATOR_H
#include <vector>
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
private:
  std::vector<Model> plainTrees;
  std::vector<Model> hillTrees;
  std::default_random_engine randomizer;
};

#endif // TREEGENERATOR_H
