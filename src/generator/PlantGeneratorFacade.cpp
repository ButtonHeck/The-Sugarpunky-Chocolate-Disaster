#include "PlantGeneratorFacade.h"

PlantGeneratorFacade::PlantGeneratorFacade()
  :
    plainPlantsGenerator(std::make_unique<PlainPlantsGenerator>()),
    grassGenerator(std::make_unique<GrassGenerator>()),
    hillTreesGenerator(std::make_unique<HillTreesGenerator>())
{}

void PlantGeneratorFacade::setup(std::vector<std::vector<float> > &baseMap, std::vector<std::vector<float> > &hillMap)
{
  plainPlantsGenerator->setup(baseMap, hillMap);
  grassGenerator->setup(baseMap, hillMap);
  hillTreesGenerator->setup(hillMap);
}

void PlantGeneratorFacade::prepareMeshesIndirectData(const glm::vec2 &cameraPositionXZ, const Frustum &viewFrustum)
{
  auto& plainPlants = getPlainModels();
  auto& hillTrees = getHillModels();
  auto& grass = getGrassModels();
  auto& plainChunks = getPlainChunks();
  auto& hillChunks = getHillTreesChunks();
  auto& grassChunks = getGrassModelChunks();

  for (unsigned int i = 0; i < plainPlants.size(); i++)
    {
      Model& model = plainPlants[i];
      model.prepareMeshesIndirectData(plainChunks, i, cameraPositionXZ, viewFrustum);
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

std::vector<Model> &PlantGeneratorFacade::getPlainModels()
{
  return plainPlantsGenerator->getModels();
}

std::vector<ModelChunk> &PlantGeneratorFacade::getPlainChunks()
{
  return plainPlantsGenerator->getChunks();
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
  plainPlantsGenerator->serialize(output);
  grassGenerator->serialize(output);
  hillTreesGenerator->serialize(output);
}

void PlantGeneratorFacade::deserialize(std::ifstream &input)
{
  plainPlantsGenerator->deserialize(input);
  grassGenerator->deserialize(input);
  hillTreesGenerator->deserialize(input);
}
