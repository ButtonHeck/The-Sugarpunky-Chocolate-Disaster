#ifndef SAVELOADMANAGER_H
#define SAVELOADMANAGER_H
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include "game/world/WorldGeneratorFacade.h"
#include "chunk/ModelChunk.h"
#include "graphics/Camera.h"

class SaveLoadManager
{
public:
  SaveLoadManager(std::shared_ptr<WorldGeneratorFacade>& worldGenerator, Camera& camera);
  bool saveToFile(const std::string& filename);
  bool loadFromFile(const std::string& filename);
private:
  std::shared_ptr<WorldGeneratorFacade>& worldGenerator;
  Camera& camera;
};

#endif // SAVELOADMANAGER_H
