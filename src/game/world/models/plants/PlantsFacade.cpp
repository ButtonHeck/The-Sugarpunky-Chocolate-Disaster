#include "game/world/models/plants/PlantsFacade.h"

PlantsFacade::PlantsFacade(Shader &renderPhongShader, Shader &renderGouraudShader)
  :
    shaders(renderPhongShader, renderGouraudShader),
    landPlantsGenerator(),
    grassGenerator(),
    hillTreesGenerator(),
    treesRenderer(),
    grassRenderer()
{}

void PlantsFacade::setup(const map2D_f &landMap, const map2D_f &hillMap, const map2D_vec3 &hillsNormalMap)
{
  prepareDistributionMap(PLANTS_DISTRIBUTION_FREQUENCY);
  landPlantsGenerator.setup(landMap, hillMap, distributionMap);
  grassGenerator.setup(landMap, hillMap, distributionMap);
  hillTreesGenerator.setup(hillMap, distributionMap, hillsNormalMap);
}

void PlantsFacade::prepareIndirectBufferData(const glm::vec2 &cameraPositionXZ, const Frustum &viewFrustum)
{
  prepareIndirectBufferData(landPlantsGenerator, cameraPositionXZ, viewFrustum);
  prepareIndirectBufferData(hillTreesGenerator, cameraPositionXZ, viewFrustum);
  prepareIndirectBufferData(grassGenerator, cameraPositionXZ, viewFrustum);
}

void PlantsFacade::updateIndirectBufferData()
{
  BENCHMARK("PlantsFacade: updateIndirectBuffer", true);
  updateIndirectBufferData(landPlantsGenerator);
  updateIndirectBufferData(hillTreesGenerator);
  updateIndirectBufferData(grassGenerator);
}

void PlantsFacade::draw(const glm::vec3 &lightDir,
                        const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                        const glm::mat4 &projectionView,
                        const glm::vec3 &viewPosition,
                        bool usePhongShading,
                        bool useShadows,
                        bool useLandBlending)
{
  if (useLandBlending)
    glEnable(GL_BLEND);
  else
    glDisable(GL_BLEND);

  shaders.updateAllPlants(usePhongShading,
                          lightDir,
                          lightSpaceMatrices,
                          projectionView,
                          viewPosition,
                          useShadows,
                          useLandBlending);
  shaders.switchToGrass(false);
  shaders.switchToLowPoly(false);
  treesRenderer.render(landPlantsGenerator.models, hillTreesGenerator.models, false);
  shaders.switchToLowPoly(true);
  treesRenderer.render(landPlantsGenerator.lowPolyModels, hillTreesGenerator.lowPolyModels, false);

  shaders.switchToGrass(true);
  shaders.updateGrass();
  shaders.switchToLowPoly(false);
  grassRenderer.render(grassGenerator.models, false);
  shaders.switchToLowPoly(true);
  grassRenderer.render(grassGenerator.lowPolyModels, false);

  if (useLandBlending)
    glDisable(GL_BLEND);
}

void PlantsFacade::drawDepthmap(bool grassCastShadow)
{
  treesRenderer.render(landPlantsGenerator.models, hillTreesGenerator.models, true);
  treesRenderer.render(landPlantsGenerator.lowPolyModels, hillTreesGenerator.lowPolyModels, true);
  if (grassCastShadow)
    grassRenderer.render(grassGenerator.models, true);
}

void PlantsFacade::serialize(std::ofstream &output)
{
  landPlantsGenerator.serialize(output);
  grassGenerator.serialize(output);
  hillTreesGenerator.serialize(output);
}

void PlantsFacade::deserialize(std::ifstream &input)
{
  landPlantsGenerator.deserialize(input);
  grassGenerator.deserialize(input);
  hillTreesGenerator.deserialize(input);
}

void PlantsFacade::prepareDistributionMap(int cycles)
{
  Generator::initializeMap(distributionMap);
  for (int cycle = 1; cycle <= cycles; cycle++)
    {
      for (unsigned int startY = 0; startY < distributionMap.size(); startY++)
        {
          for (unsigned int startX = 0; startX < distributionMap[0].size(); startX++)
            {
              if (rand() % (cycles * 5) == 0)
                {
                  unsigned int yBorder = startY + cycle - 1;
                  if (yBorder >= distributionMap.size())
                    yBorder = distributionMap.size() - 1;
                  unsigned int xBorder = startX + cycle - 1;
                  if (xBorder >= distributionMap[0].size())
                    xBorder = distributionMap.size() - 1;
                  for (unsigned int y = startY; y <= yBorder; y++)
                    {
                      for (unsigned int x = startX; x <= xBorder; x++)
                        {
                          if (distributionMap[y][x] < cycles)
                            distributionMap[y][x]++;
                        }
                    }
                }
            }
        }
    }
}

void PlantsFacade::prepareIndirectBufferData(PlantGenerator &generator, const glm::vec2 &cameraPositionXZ, const Frustum &viewFrustum)
{
  auto& models = generator.models;
  auto& lowPolyModels = generator.lowPolyModels;
  auto& chunks = generator.chunks;
  for (unsigned int modelIndex = 0; modelIndex < models.size(); modelIndex++)
    {
      Model& model = models[modelIndex];
      model.prepareIndirectBufferData(chunks, modelIndex, cameraPositionXZ, viewFrustum,
                                      PlantGenerator::LOADING_DISTANCE_UNITS_SQUARE,
                                      PlantGenerator::LOADING_DISTANCE_UNITS_SHADOW_SQUARE,
                                      PlantGenerator::LOADING_DISTANCE_UNITS_LOWPOLY_SQUARE);
      Model& lowPolyModel = lowPolyModels[modelIndex];
      lowPolyModel.prepareIndirectBufferData(chunks, modelIndex, cameraPositionXZ, viewFrustum,
                                             PlantGenerator::LOADING_DISTANCE_UNITS_SQUARE,
                                             PlantGenerator::LOADING_DISTANCE_UNITS_SHADOW_SQUARE,
                                             PlantGenerator::LOADING_DISTANCE_UNITS_LOWPOLY_SQUARE);
    }
}

void PlantsFacade::updateIndirectBufferData(PlantGenerator &generator)
{
  auto& models = generator.models;
  auto& lowPolyModels = generator.lowPolyModels;
  for (unsigned int modelIndex = 0; modelIndex < models.size(); modelIndex++)
    {
      Model& model = models[modelIndex];
      model.updateIndirectBufferData();
      Model& lowPolyModel = lowPolyModels[modelIndex];
      lowPolyModel.updateIndirectBufferData();
    }
}
