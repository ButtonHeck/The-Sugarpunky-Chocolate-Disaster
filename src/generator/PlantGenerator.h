#ifndef PLANTGENERATOR_H
#define PLANTGENERATOR_H
#include <vector>
#include <fstream>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <chrono>
#include "game/Settings.h"
#include "model/Model.h"
#include "chunk/ModelChunk.h"

class PlantGenerator
{
public:
  PlantGenerator(int numGrassModels);
  ~PlantGenerator();

  //plain plants
  void setupPlainModels(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap);
  void updatePlainModels(std::vector<glm::mat4*>& models, unsigned int* numAllTrees);
  std::vector<glm::mat4*>& getPlainPlantsMatrices();
  unsigned int getNumPlainPlants(int i);
  std::vector<Model>& getPlainPlants();
  std::vector<ModelChunk>& getPlainPlantsModelChunks();

  //hill trees
  void setupHillModels(std::vector<std::vector<float>>& hillMap);
  void updateHillModels(std::vector<glm::mat4*>& models, unsigned int* numAllTrees);
  std::vector<glm::mat4*>& getHillTreesMatrices();
  unsigned int getNumHillTrees(int i);
  std::vector<Model>& getHillTrees();
  std::vector<ModelChunk>& getHillTreeModelChunks();

  //save/load routine
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);

private:
  //plain plants
  std::vector<Model> plainPlants;
  std::vector<glm::mat4*> plainPlantsMatrices;
  unsigned int* numPlainPlants;
  int numGrassModels;
  bool plainPlantsAlreadyCreated = false;
  std::vector<ModelChunk> plainPlantsModelChunks;

  //hill trees
  std::vector<Model> hillTrees;
  std::vector<glm::mat4*> hillTreesMatrices;
  unsigned int* numHillTrees;
  bool hillTreesAlreadyCreated = false;
  std::vector<ModelChunk> hillTreesModelChunks;

  //plants distibution
  std::default_random_engine randomizer;
};

#endif // PLANTGENERATOR_H
