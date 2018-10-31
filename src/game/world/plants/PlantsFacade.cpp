#include "game/world/plants/PlantsFacade.h"

PlantsFacade::PlantsFacade(Shader &renderPhongShader, Shader &renderGouraudShader, Shader &shadowShader)
  :
    shaders(renderPhongShader, renderGouraudShader, shadowShader),
    landPlantsGenerator(std::make_unique<LandPlantsGenerator>()),
    grassGenerator(std::make_unique<GrassGenerator>()),
    hillTreesGenerator(std::make_unique<HillTreesGenerator>()),
    renderer()
{}

void PlantsFacade::setup(map2D_f &baseMap, map2D_f &hillMap)
{
  prepareDistributionMap(MODELS_DISTRIBUTION_FREQ);
  landPlantsGenerator->setup(baseMap, hillMap, distributionMap);
  grassGenerator->setup(baseMap, hillMap, distributionMap);
  hillTreesGenerator->setup(hillMap, distributionMap);
}

void PlantsFacade::prepareMeshesIndirectData(const glm::vec2 &cameraPositionXZ, const Frustum &viewFrustum)
{
  auto& landPlants = landPlantsGenerator->getModels();
  auto& hillTrees = hillTreesGenerator->getModels();
  auto& grass = grassGenerator->getModels();
  auto& landChunks = landPlantsGenerator->getChunks();
  auto& hillChunks = hillTreesGenerator->getChunks();
  auto& grassChunks = grassGenerator->getChunks();

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

void PlantsFacade::updateIndirectBufferData()
{
  auto& landPlants = landPlantsGenerator->getModels();
  auto& hillTrees = hillTreesGenerator->getModels();
  auto& grass = grassGenerator->getModels();

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

void PlantsFacade::draw(glm::mat4& projectionView,
                        glm::vec3& viewPosition,
                        bool usePhongShading,
                        bool useShadowEmphasize,
                        bool useShadows,
                        bool useFlatBlending)
{
  shaders.update(projectionView,
                 viewPosition,
                 useShadowEmphasize,
                 useShadows,
                 useFlatBlending);
  {
    BENCHMARK("Renderer: draw models", true);
    renderer.render(landPlantsGenerator->getModels(),
                    hillTreesGenerator->getModels(),
                    grassGenerator->getModels(),
                    usePhongShading ? shaders.renderPhongShader : shaders.renderGouraudShader,
                    true,
                    true,
                    useFlatBlending);
  }
}

void PlantsFacade::drawDepthmap()
{
  BENCHMARK("Renderer: draw models depthmap", true);
  renderer.render(landPlantsGenerator->getModels(),
                  hillTreesGenerator->getModels(),
                  grassGenerator->getModels(),
                  shaders.shadowShader,
                  false,
                  false,
                  false);
}

void PlantsFacade::serialize(std::ofstream &output)
{
  landPlantsGenerator->serialize(output);
  grassGenerator->serialize(output);
  hillTreesGenerator->serialize(output);
}

void PlantsFacade::deserialize(std::ifstream &input)
{
  landPlantsGenerator->deserialize(input);
  grassGenerator->deserialize(input);
  hillTreesGenerator->deserialize(input);
}

void PlantsFacade::prepareDistributionMap(int cycles)
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
