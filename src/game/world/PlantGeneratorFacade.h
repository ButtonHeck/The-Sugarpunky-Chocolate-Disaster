#ifndef PLANTGENERATORFACADE_H
#define PLANTGENERATORFACADE_H
#include <memory>
#include "game/world/plants/LandPlantsGenerator.h"
#include "game/world/plants/GrassGenerator.h"
#include "game/world/plants/HillTreesGenerator.h"
#include "graphics/Frustum.h"

class PlantGeneratorFacade
{
public:
  PlantGeneratorFacade();
  virtual ~PlantGeneratorFacade() = default;
  void setup(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap);
  void prepareMeshesIndirectData(const glm::vec2& cameraPositionXZ, const Frustum& viewFrustum);

  //plain plants
  std::vector<Model>& getPlainModels();
  std::vector<ModelChunk>& getPlainChunks();

  //grass
  std::vector<Model>& getGrassModels();
  std::vector<ModelChunk>& getGrassModelChunks();

  //hill trees
  std::vector<Model>& getHillModels();
  std::vector<ModelChunk>& getHillTreesChunks();

  //save/load routine
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);
private:
  std::unique_ptr<LandPlantsGenerator> plainPlantsGenerator;
  std::unique_ptr<GrassGenerator> grassGenerator;
  std::unique_ptr<HillTreesGenerator> hillTreesGenerator;
};

#endif // PLANTGENERATORFACADE_H
