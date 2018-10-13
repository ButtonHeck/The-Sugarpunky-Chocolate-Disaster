#ifndef PLANTGENERATOR_H
#define PLANTGENERATOR_H
#include <vector>
#include <fstream>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include "util/Settings.h"
#include "model/Model.h"
#include "game/world/plants/ModelChunk.h"

class PlantGenerator
{
public:
  PlantGenerator();
  virtual ~PlantGenerator();
  std::vector<Model>& getModels();
  std::vector<ModelChunk>& getChunks();
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);
protected:
  void setupModelChunks();
  void updateMatrices(std::vector<glm::mat4*>& newModels, unsigned int* numAllTrees);
  std::vector<std::vector<glm::mat4>> substituteMatricesStorage();
  void loadMatrices(std::vector<std::vector<glm::mat4>>& newMatrices);
  std::vector<Model> models;
  std::vector<glm::mat4*> matrices;
  std::unique_ptr<unsigned int[]> numPlants;
  std::vector<ModelChunk> chunks;
  std::default_random_engine randomizer;
};

#endif // PLANTGENERATOR_H
