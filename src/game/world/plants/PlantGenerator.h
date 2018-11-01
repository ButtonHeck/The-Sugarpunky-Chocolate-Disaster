#ifndef PLANTGENERATOR_H
#define PLANTGENERATOR_H
#include <vector>
#include <fstream>
#include <memory>
#include <random>
#include "util/Settings.h"
#include "model/Model.h"
#include "game/world/plants/ModelChunk.h"

class PlantGenerator
{
public:
  PlantGenerator();
  virtual ~PlantGenerator();
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);

protected:
  friend class PlantsFacade;
  void setupModelChunks();
  void updateMatrices(std::vector<glm::mat4*>& newModels, unsigned int* numAllTrees);
  map2D_mat4 substituteMatricesStorage();
  void loadMatrices(map2D_mat4& newMatrices);

  std::vector<Model> models;
  std::vector<glm::mat4*> matrices;
  std::unique_ptr<unsigned int[]> numPlants;
  std::vector<ModelChunk> chunks;
  std::default_random_engine randomizer;
};

#endif // PLANTGENERATOR_H
