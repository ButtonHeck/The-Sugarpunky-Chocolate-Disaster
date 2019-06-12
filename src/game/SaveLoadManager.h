#pragma once

#include <string>

class Camera;
class Scene;

class SaveLoadManager
{
public:
  SaveLoadManager(Scene& scene, Camera& camera, Camera& shadowCamera) noexcept;
  bool saveToFile(const std::string& filename);
  bool loadFromFile(const std::string& filename);

private:
  Scene& scene;
  Camera& camera;
  Camera& shadowCamera;
};
