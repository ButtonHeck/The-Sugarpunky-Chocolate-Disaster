#ifndef SAVELOADMANAGER_H
#define SAVELOADMANAGER_H
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include "generator/BaseMapGenerator.h"
#include "generator/HillsMapGenerator.h"
#include "generator/WaterMapGenerator.h"
#include "generator/PlantGeneratorFacade.h"
#include "generator/BuildableMapGenerator.h"
#include "chunk/ModelChunk.h"
#include "graphics/Camera.h"

class SaveLoadManager
{
public:
  SaveLoadManager(std::shared_ptr<BaseMapGenerator>& baseGenerator,
                  std::shared_ptr<HillsMapGenerator>& hillGenerator,
                  std::shared_ptr<WaterMapGenerator>& waterGenerator,
                  std::shared_ptr<BuildableMapGenerator>& buildableGenerator,
                  std::shared_ptr<PlantGeneratorFacade>& plantGeneratorFacade,
                  Camera& camera);
  bool saveToFile(const std::string& filename);
  bool loadFromFile(const std::string& filename);
  void update(std::shared_ptr<BaseMapGenerator>& baseGenerator,
              std::shared_ptr<HillsMapGenerator>& hillGenerator,
              std::shared_ptr<WaterMapGenerator>& waterGenerator,
              std::shared_ptr<BuildableMapGenerator>& buildableGenerator,
              std::shared_ptr<PlantGeneratorFacade>& plantGeneratorFacade,
              Camera& camera);
private:
  std::shared_ptr<BaseMapGenerator>& baseGenerator;
  std::shared_ptr<HillsMapGenerator>& hillGenerator;
  std::shared_ptr<WaterMapGenerator>& waterGenerator;
  std::shared_ptr<BuildableMapGenerator>& buildableGenerator;
  std::shared_ptr<PlantGeneratorFacade>& plantGeneratorFacade;
  Camera& camera;
};

#endif // SAVELOADMANAGER_H
