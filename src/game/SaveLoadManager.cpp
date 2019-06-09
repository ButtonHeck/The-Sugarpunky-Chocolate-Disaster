#include "SaveLoadManager"
#include "Camera"
#include "Scene"
#include "Logger"

SaveLoadManager::SaveLoadManager(Scene &scene, Camera& camera, Camera &shadowCamera)
  :
    scene(scene),
    camera(camera),
    shadowCamera(shadowCamera)
{}

bool SaveLoadManager::saveToFile(const std::string &filename)
{
  std::ofstream output(filename);
  if (!output)
    {
      Logger::log("Could not open file: %\n", filename.c_str());
      return false;
    }
  scene.serialize(output);
  camera.serialize(output);
  output.close();
  return true;
}

bool SaveLoadManager::loadFromFile(const std::string &filename)
{
  std::ifstream input(filename);
  if (!input)
    {
      Logger::log("Could not open file: %\n", filename.c_str());
      return false;
    }
  scene.deserialize(input);
  camera.deserialize(input);
  shadowCamera = camera;
  input.close();
  return true;
}
