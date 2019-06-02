#ifndef SAVELOADMANAGER_H
#define SAVELOADMANAGER_H
#include <fstream>
#include <vector>
#include "../../src/game/world/Scene.h"
#include "../../src/util/Logger.h"

class Camera;

class SaveLoadManager
{
public:
  SaveLoadManager(Scene& scene, Camera& camera, Camera& shadowCamera);
  bool saveToFile(const std::string& filename);
  bool loadFromFile(const std::string& filename);
private:
  Scene& scene;
  Camera& camera;
  Camera& shadowCamera;
};

#endif // SAVELOADMANAGER_H
