#ifndef PLANTGENERATOR_H
#define PLANTGENERATOR_H
#include <vector>
#include <fstream>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include "game/Settings.h"
#include "model/Model.h"
#include "chunk/ModelChunk.h"

class PlantGenerator
{
public:
  PlantGenerator(int numGrassModels);
  ~PlantGenerator();
  void setup(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap);

  //plain plants
  std::vector<Model>& getPlainPlants();
  std::vector<ModelChunk>& getPlainPlantsModelChunks();

  //hill trees
  std::vector<Model>& getHillTrees();
  std::vector<ModelChunk>& getHillTreeModelChunks();

  //save/load routine
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);

private:
  //both plain plants and hill trees
  void setupModelChunks();

  //plain plants
  void setupPlainMatrices(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap);
  void updatePlain(std::vector<glm::mat4*>& models, unsigned int* numAllTrees);
  std::vector<Model> plainPlants;
  std::vector<glm::mat4*> plainPlantsMatrices;
  unsigned int* numPlainPlants = nullptr;
  int numGrassModels;
  std::vector<ModelChunk> plainPlantsModelChunks;

  //hill trees
  void setupHillMatrices(std::vector<std::vector<float>>& hillMap);
  void updateHill(std::vector<glm::mat4*>& models, unsigned int* numAllTrees);
  std::vector<Model> hillTrees;
  std::vector<glm::mat4*> hillTreesMatrices;
  unsigned int* numHillTrees = nullptr;
  std::vector<ModelChunk> hillTreesModelChunks;

  //plants distibution
  std::default_random_engine randomizer;
};

#endif // PLANTGENERATOR_H
