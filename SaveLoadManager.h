#ifndef SAVELOADMANAGER_H
#define SAVELOADMANAGER_H
#include <fstream>
#include <string>
#include <vector>
#include "TerrainTile.h"
#include "BaseMapGenerator.h"
#include "HillsMapGenerator.h"
#include "WaterMapGenerator.h"
#include "TreeGenerator.h"

class SaveLoadManager
{
public:
  SaveLoadManager(BaseMapGenerator& baseGenerator, HillsMapGenerator& hillGenerator, WaterMapGenerator& waterGenerator);
  bool saveToFile(const std::string& filename);
  bool loadFromFile(const std::string& filename);
  void setTreeGenerator(TreeGenerator& treeGenerator);
private:
  BaseMapGenerator& baseGenerator;
  HillsMapGenerator& hillGenerator;
  WaterMapGenerator& waterGenerator;
  TreeGenerator* treeGenerator;
  std::vector<std::vector<float>>& baseMap;
  std::vector<std::vector<float>>& hillMap;
  std::vector<std::vector<float>>& waterMap;
  std::vector<std::vector<float>>& chunkMap;
};

#endif // SAVELOADMANAGER_H
