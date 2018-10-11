#include "game/SaveLoadManager.h"

SaveLoadManager::SaveLoadManager(std::shared_ptr<BaseMapGenerator>& baseGenerator,
                                 std::shared_ptr<HillsMapGenerator>& hillGenerator,
                                 std::shared_ptr<WaterMapGenerator>& waterGenerator,
                                 std::shared_ptr<BuildableMapGenerator>& buildableGenerator,
                                 std::shared_ptr<PlantGeneratorFacade>& plantGeneratorFacade,
                                 Camera& camera)
  :
    baseGenerator(baseGenerator),
    hillGenerator(hillGenerator),
    waterGenerator(waterGenerator),
    buildableGenerator(buildableGenerator),
    plantGeneratorFacade(plantGeneratorFacade),
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
  baseGenerator->serialize(output);
  hillGenerator->serialize(output);
  waterGenerator->serialize(output);
  plantGeneratorFacade->serialize(output);
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
  baseGenerator->deserialize(input);
  hillGenerator->deserialize(input);
  waterGenerator->deserialize(input);
  plantGeneratorFacade->deserialize(input);
  camera.deserialize(input);
  input.close();
  return true;
}

void SaveLoadManager::update(std::shared_ptr<BaseMapGenerator> &baseGenerator,
                             std::shared_ptr<HillsMapGenerator> &hillGenerator,
                             std::shared_ptr<WaterMapGenerator> &waterGenerator,
                             std::shared_ptr<BuildableMapGenerator> &buildableGenerator,
                             std::shared_ptr<PlantGeneratorFacade> &plantGeneratorFacade,
                             Camera &camera)
{
  this->baseGenerator = baseGenerator;
  this->hillGenerator = hillGenerator;
  this->waterGenerator = waterGenerator;
  this->buildableGenerator = buildableGenerator;
  this->plantGeneratorFacade = plantGeneratorFacade;
  this->camera = camera;
}
