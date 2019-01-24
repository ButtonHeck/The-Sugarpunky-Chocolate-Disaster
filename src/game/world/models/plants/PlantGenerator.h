#ifndef PLANTGENERATOR_H
#define PLANTGENERATOR_H
#include <vector>
#include <fstream>
#include <memory>
#include <random>
#include "game/world/models/Model.h"
#include "game/world/models/ModelChunk.h"

class PlantGenerator
{
public:
  PlantGenerator();
  virtual ~PlantGenerator();
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);

  constexpr static unsigned int LOADING_DISTANCE_CHUNKS = 8;
  constexpr static unsigned int LOADING_DISTANCE_UNITS = CHUNK_SIZE * LOADING_DISTANCE_CHUNKS;
  constexpr static unsigned int LOADING_DISTANCE_UNITS_SQUARE = LOADING_DISTANCE_UNITS * LOADING_DISTANCE_UNITS;
  constexpr static unsigned int LOADING_DISTANCE_CHUNKS_LOWPOLY = 32;
  constexpr static unsigned int LOADING_DISTANCE_UNITS_LOWPOLY = CHUNK_SIZE * LOADING_DISTANCE_CHUNKS_LOWPOLY;
  constexpr static unsigned int LOADING_DISTANCE_UNITS_LOWPOLY_SQUARE = LOADING_DISTANCE_UNITS_LOWPOLY * LOADING_DISTANCE_UNITS_LOWPOLY;
  constexpr static unsigned int LOADING_DISTANCE_CHUNKS_SHADOW = 16;
  constexpr static unsigned int LOADING_DISTANCE_UNITS_SHADOW = CHUNK_SIZE * LOADING_DISTANCE_CHUNKS_SHADOW;
  constexpr static unsigned int LOADING_DISTANCE_UNITS_SHADOW_SQUARE = LOADING_DISTANCE_UNITS_SHADOW * LOADING_DISTANCE_UNITS_SHADOW;

protected:
  friend class PlantsFacade;
  void setupModelChunks();
  void updateMatrices(std::vector<glm::mat4*>& newModels, unsigned int* numAllTrees);
  map2D_mat4 substituteMatricesStorage();
  void loadMatrices(map2D_mat4& newMatrices);

  std::vector<Model> models;
  std::vector<Model> lowPolyModels;
  std::vector<glm::mat4*> matrices;
  std::unique_ptr<unsigned int[]> numPlants;
  std::vector<ModelChunk> chunks;
  std::default_random_engine randomizer;
};

#endif // PLANTGENERATOR_H
