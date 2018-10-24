#include "game/world/PlantGeneratorFacade.h"

PlantGeneratorFacade::PlantGeneratorFacade()
  :
    landPlantsGenerator(std::make_unique<LandPlantsGenerator>()),
    grassGenerator(std::make_unique<GrassGenerator>()),
    hillTreesGenerator(std::make_unique<HillTreesGenerator>())
{}

void PlantGeneratorFacade::setup(std::vector<std::vector<float> > &baseMap, std::vector<std::vector<float> > &hillMap)
{
  prepareDistributionMap(MODELS_DISTRIBUTION_FREQ);
  landPlantsGenerator->setup(baseMap, hillMap, distributionMap);
  grassGenerator->setup(baseMap, hillMap, distributionMap);
  hillTreesGenerator->setup(hillMap, distributionMap);
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

void PlantGeneratorFacade::prepareDistributionMap(int cycles)
{
  initializeMap(distributionMap);
  for (int cycle = 1; cycle <= cycles; cycle++)
    {
      for (unsigned int y = 0; y < distributionMap.size(); y++)
        {
          for (unsigned int x = 0; x < distributionMap[0].size(); x++)
            {
              if (rand() % (cycles * 5) == 0)
                {
                  unsigned int yBorder = y + cycle - 1;
                  if (yBorder >= distributionMap.size())
                    yBorder = distributionMap.size() - 1;
                  unsigned int xBorder = x + cycle - 1;
                  if (xBorder >= distributionMap[0].size())
                    xBorder = distributionMap.size() - 1;
                  for (unsigned int y2 = y; y2 <= yBorder; y2++)
                    {
                      for (unsigned int x2 = x; x2 <= xBorder; x2++)
                        {
                          if (distributionMap[y2][x2] < cycles)
                            distributionMap[y2][x2]++;
                        }
                    }
                }
            }
        }
    }

}
