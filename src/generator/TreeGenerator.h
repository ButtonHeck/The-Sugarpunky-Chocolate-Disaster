#ifndef TREEGENERATOR_H
#define TREEGENERATOR_H
#include <vector>
#include <fstream>
#include "src/model/Model.h"
#include "src/graphics/Shader.h"
#include "src/game/Settings.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include "src/chunk/ModelChunk.h"
#include "src/graphics/Camera.h"

class TreeGenerator
{
public:
  TreeGenerator(std::vector<std::string> plainModelsPaths,
                std::vector<std::string> hillModelsPaths,
                TextureLoader& textureLoader,
                std::vector<ModelChunk>& treeModelChunks,
                std::vector<ModelChunk>& hillTreeModelChunks);
  void setupPlainModels(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap);
  void updatePlainModels(std::vector<glm::mat4*>& models, unsigned int* numAllTrees);
  void setupHillModels(std::vector<std::vector<float>>& hillMap);
  void updateHillModels(std::vector<glm::mat4*>& models, unsigned int* numAllTrees);
  std::vector<glm::mat4*>& getTreeModels();
  std::vector<glm::mat4*>& getHillTreeModels();
  unsigned int getNumTrees(int i);
  unsigned int getNumHillTrees(int i);
  std::vector<Model>& getPlainTrees();
  std::vector<Model>& getHillTrees();
  std::vector<ModelChunk>& getTreeModelChunks();
  std::vector<ModelChunk>& getHillTreeModelChunks();
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
  std::vector<ModelChunk>& treeModelChunks;
  std::vector<ModelChunk>& hillTreeModelChunks;
};

#endif // TREEGENERATOR_H
