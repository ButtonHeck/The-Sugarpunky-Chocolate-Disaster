#include "game/SaveLoadManager.h"

SaveLoadManager::SaveLoadManager(std::shared_ptr<WorldGeneratorFacade>& worldGenerator, Camera& camera)
  :
    worldGenerator(worldGenerator),
    camera(camera)
{}

bool SaveLoadManager::saveToFile(const std::string &filename)
{
  std::ofstream output(filename);
  if (!output)
    {
      std::cerr << "Could not open file: " << filename << std::endl;
      return false;
    }
  worldGenerator->serialize(output);
  camera.serialize(output);
  output.close();
  return true;
}

bool SaveLoadManager::loadFromFile(const std::string &filename)
{
  std::ifstream input(filename);
  if (!input)
    {
      std::cerr << "Could not open file: " << filename << std::endl;
      return false;
    }
  worldGenerator->deserialize(input);
  camera.deserialize(input);
  input.close();
  return true;
}
