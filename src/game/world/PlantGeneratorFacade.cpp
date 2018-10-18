#include "game/world/PlantGeneratorFacade.h"

PlantGeneratorFacade::PlantGeneratorFacade()
  :
    landPlantsGenerator(std::make_unique<LandPlantsGenerator>()),
    grassGenerator(std::make_unique<GrassGenerator>()),
    hillTreesGenerator(std::make_unique<HillTreesGenerator>())
{}

void PlantGeneratorFacade::setup(std::vector<std::vector<float> > &baseMap, std::vector<std::vector<float> > &hillMap)
{
  landPlantsGenerator->setup(baseMap, hillMap);
  grassGenerator->setup(baseMap, hillMap);
  hillTreesGenerator->setup(hillMap);
}

void PlantGeneratorFacade::prepareMeshesIndirectData(const glm::vec2 &cameraPositionXZ, const Frustum &viewFrustum)
{
  auto& landPlants = getLandModels();
  auto& hillTrees = getHillModels();
  auto& grass = getGrassModels();
  auto& landChunks = getLandChunks();
  auto& hillChunks = getHillTreesChunks();
  auto& grassChunks = getGrassModelChunks();

  for (unsigned int i = 0; i < landPlants.size(); i++)
    {
      Model& model = landPlants[i];
      model.prepareMeshesIndirectData(landChunks, i, cameraPositionXZ, viewFrustum);
    }
  for (unsigned int i = 0; i < hillTrees.size(); i++)
    {
      Model& model = hillTrees[i];
      model.prepareMeshesIndirectData(hillChunks, i, cameraPositionXZ, viewFrustum);
    }
  for (unsigned int i = 0; i < grass.size(); i++)
    {
      Model& model = grass[i];
      model.prepareMeshesIndirectData(grassChunks, i, cameraPositionXZ, viewFrustum);
    }
}

void PlantGeneratorFacade::updateIndirectBufferData()
{
  auto& landPlants = getLandModels();
  auto& hillTrees = getHillModels();
  auto& grass = getGrassModels();

  for (unsigned int i = 0; i < landPlants.size(); i++)
    {
      Model& model = landPlants[i];
      model.updateIndirectBufferData();
    }
  for (unsigned int i = 0; i < hillTrees.size(); i++)
    {
      Model& model = hillTrees[i];
      model.updateIndirectBufferData();
    }
  for (unsigned int i = 0; i < grass.size(); i++)
    {
      Model& model = grass[i];
      model.updateIndirectBufferData();
    }
}

std::vector<Model> &PlantGeneratorFacade::getLandModels()
{
  return landPlantsGenerator->getModels();
}

std::vector<ModelChunk> &PlantGeneratorFacade::getLandChunks()
{
  return landPlantsGenerator->getChunks();
}

std::vector<Model> &PlantGeneratorFacade::getGrassModels()
{
  return grassGenerator->getModels();
}

std::vector<ModelChunk> &PlantGeneratorFacade::getGrassModelChunks()
{
  return grassGenerator->getChunks();
}

std::vector<Model> &PlantGeneratorFacade::getHillModels()
{
  return hillTreesGenerator->getModels();
}

std::vector<ModelChunk> &PlantGeneratorFacade::getHillTreesChunks()
{
  return hillTreesGenerator->getChunks();
}

void PlantGeneratorFacade::serialize(std::ofstream &output)
{
  landPlantsGenerator->serialize(output);
  grassGenerator->serialize(output);
  hillTreesGenerator->serialize(output);
}

void PlantGeneratorFacade::deserialize(std::ifstream &input)
{
  landPlantsGenerator->deserialize(input);
  grassGenerator->deserialize(input);
  hillTreesGenerator->deserialize(input);
}
