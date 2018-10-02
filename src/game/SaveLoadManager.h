#ifndef SAVELOADMANAGER_H
#define SAVELOADMANAGER_H
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include "generator/BaseMapGenerator.h"
#include "generator/HillsMapGenerator.h"
#include "generator/WaterMapGenerator.h"
#include "generator/PlantGenerator.h"
#include "generator/BuildableMapGenerator.h"
#include "chunk/ModelChunk.h"
#include "graphics/Camera.h"

class SaveLoadManager
{
public:
  SaveLoadManager(std::shared_ptr<BaseMapGenerator> baseGenerator,
                  std::shared_ptr<HillsMapGenerator> hillGenerator,
                  std::shared_ptr<WaterMapGenerator> waterGenerator,
                  std::shared_ptr<BuildableMapGenerator> buildableGenerator, Camera& camera);
  bool saveToFile(const std::string& filename);
  bool loadFromFile(const std::string& filename);
  void setTreeGenerator(const std::shared_ptr<PlantGenerator> treeGenerator);
private:
  std::shared_ptr<BaseMapGenerator> baseGenerator;
  std::shared_ptr<HillsMapGenerator> hillGenerator;
  std::shared_ptr<WaterMapGenerator> waterGenerator;
  std::shared_ptr<BuildableMapGenerator> buildableGenerator;
  std::shared_ptr<PlantGenerator> treeGenerator;
  Camera& camera;
  std::vector<std::vector<float>>& baseMap;
  std::vector<std::vector<float>>& hillMap;
  std::vector<std::vector<float>>& waterMap;
  std::vector<std::vector<float>>& chunkMap;
};

#endif // SAVELOADMANAGER_H
