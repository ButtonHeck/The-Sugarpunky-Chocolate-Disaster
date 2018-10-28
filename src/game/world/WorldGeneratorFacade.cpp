#include "game/world/WorldGeneratorFacade.h"

WorldGeneratorFacade::WorldGeneratorFacade(ShaderManager &shaderManager, Renderer &renderer, Options &options, TextureManager &textureManager)
  :
    shaderManager(shaderManager),
    renderer(renderer),
    options(options),
    textureManager(textureManager)
{
  waterGenerator = std::make_shared<WaterGenerator>(shaderManager.get(SHADER_WATER_CULLING));
  hillsFacade = std::make_unique<HillsFacade>(shaderManager.get(SHADER_HILLS), shaderManager.get(SHADER_HILLS_CULLING), waterGenerator->getMap());
  landFacade = std::make_unique<LandFacade>(shaderManager.get(SHADER_LAND));
  shoreFacade = std::make_unique<ShoreFacade>(shaderManager.get(SHADER_SHORE), waterGenerator->getMap());
  buildableFacade = std::make_unique<BuildableFacade>(shaderManager.get(SHADER_BUILDABLE), shaderManager.get(SHADER_SELECTED));
  plantGeneratorFacade = std::make_shared<PlantGeneratorFacade>();
  underwaterFacade = std::make_unique<UnderwaterFacade>(shaderManager.get(SHADER_UNDERWATER));
}

void WorldGeneratorFacade::setup()
{
  waterGenerator->setup();
  hillsFacade->setup();
  shoreFacade->setup();
  landFacade->setup(shoreFacade->getMap());
  waterGenerator->setupConsiderTerrain();
  buildableFacade->setup(landFacade->getMap(), hillsFacade->getMap());
  plantGeneratorFacade->setup(landFacade->getMap(), hillsFacade->getMap());
  textureManager.createUnderwaterReliefTexture(waterGenerator);
}

void WorldGeneratorFacade::recreate()
{
  landFacade.reset(new LandFacade(shaderManager.get(SHADER_LAND)));
  initializeMap(waterGenerator->getMap());
  initializeMap(hillsFacade->getMap());
  setup();
}

void WorldGeneratorFacade::load()
{
  hillsFacade->createTilesAndBufferData();
  shoreFacade->setup();
  landFacade->setup(shoreFacade->getMap());
  waterGenerator->setupConsiderTerrain();
  buildableFacade->setup(landFacade->getMap(), hillsFacade->getMap());
  textureManager.createUnderwaterReliefTexture(waterGenerator);
}

void WorldGeneratorFacade::serialize(std::ofstream &output)
{
  landFacade->serialize(output);
  shoreFacade->serialize(output);
  hillsFacade->serialize(output);
  waterGenerator->serialize(output);
  plantGeneratorFacade->serialize(output);
}

void WorldGeneratorFacade::deserialize(std::ifstream &input)
{
  landFacade->deserialize(input);
  shoreFacade->deserialize(input);
  hillsFacade->deserialize(input);
  waterGenerator->deserialize(input);
  plantGeneratorFacade->deserialize(input);
}

void WorldGeneratorFacade::drawWorld(glm::mat4& projectionView,
                                     glm::mat4 &skyProjectionView,
                                     Frustum &viewFrustum,
                                     Camera& camera,
                                     MouseInputManager& mouseInput)
{
  this->projectionView = projectionView;
  glm::vec3 viewPosition = camera.getPosition();

  hillsFacade->draw(options.get(OPT_HILLS_CULLING), options.get(OPT_USE_SHADOWS), projectionView, viewPosition, viewFrustum);
  if (options.get(OPT_DRAW_LAND))
    landFacade->draw(projectionView, options.get(OPT_USE_SHADOWS), viewFrustum, textureManager.get(TEX_LAND));
  underwaterFacade->draw(projectionView);
  shoreFacade->draw(projectionView, options.get(OPT_USE_SHADOWS));
  drawPlants(viewPosition);
  if (options.get(OPT_DRAW_BUILDABLE))
    buildableFacade->drawBuildable(projectionView);
  if (options.get(OPT_SHOW_CURSOR))
    {
      mouseInput.updateCursorMappingCoordinates(camera, landFacade->getMap(), hillsFacade->getMap(), buildableFacade->getMap());
      buildableFacade->drawSelected(mouseInput, projectionView);
    }
  drawWater(viewPosition, viewFrustum);
  drawAmbient(skyProjectionView, viewPosition);
}

void WorldGeneratorFacade::drawWorldDepthmap()
{
  glClear(GL_DEPTH_BUFFER_BIT);
  glDisable(GL_CULL_FACE); //or set front face culling
  drawTerrainDepthmap();
  drawPlantsDepthmap();
  glEnable(GL_CULL_FACE); //or set back face culling
}

void WorldGeneratorFacade::drawPlants(glm::vec3& viewPosition)
{
  if (options.get(OPT_DRAW_TREES))
    {
      shaderManager.updateModelShader(projectionView, viewPosition,
                                      options.get(OPT_TREES_SHADOW_EMPHASIZE),
                                      options.get(OPT_USE_SHADOWS),
                                      options.get(OPT_MODELS_FLAT_BLENDING));
      {
        BENCHMARK("Renderer: draw models", true);
        renderer.renderPlants(plantGeneratorFacade,
                           options.get(OPT_MODELS_PHONG_SHADING) ? shaderManager.get(SHADER_MODELS_PHONG) : shaderManager.get(SHADER_MODELS),
                           true,
                           true,
                           options.get(OPT_MODELS_FLAT_BLENDING));
      }
    }
}

void WorldGeneratorFacade::drawWater(glm::vec3 &viewPosition, Frustum &viewFrustum)
{
  if (options.get(OPT_DRAW_WATER))
    {
      shaderManager.updateWaterShaders(options.get(OPT_WATER_CULLING), projectionView, viewPosition, viewFrustum);
      {
        BENCHMARK("Renderer: draw water (full func)", true);
        renderer.renderWater(options.get(OPT_WATER_CULLING), waterGenerator, shaderManager.get(SHADER_WATER_CULLING), shaderManager.get(SHADER_WATER));
      }
    }
}

void WorldGeneratorFacade::drawAmbient(glm::mat4 &skyProjectionView, glm::vec3 &viewPosition)
{
  drawSun(skyProjectionView);
  drawSkybox(skyProjectionView, viewPosition);
}

void WorldGeneratorFacade::drawSkybox(glm::mat4& skyProjectionView, glm::vec3& viewPosition)
{
  shaderManager.updateSkyShader(skyProjectionView, viewPosition, 2, true);
  renderer.renderSkybox(&skybox);
  shaderManager.updateSkyShader(skyProjectionView, viewPosition, 0, false);
  renderer.renderSkybox(&skybox);
  shaderManager.updateSkyShader(skyProjectionView, viewPosition, 1, false);
  renderer.renderSkybox(&skybox);
}

void WorldGeneratorFacade::drawSun(glm::mat4& skyProjectionView)
{
  glm::mat4 model = theSun.move(glfwGetTime());
  shaderManager.updateSunShader(skyProjectionView, model);
  renderer.renderSun(&theSun);
}

void WorldGeneratorFacade::drawTerrainDepthmap()
{
  shaderManager.get(SHADER_SHADOW_TERRAIN).use();
  hillsFacade->drawDepthmap();
  shoreFacade->drawDepthmap();
}

void WorldGeneratorFacade::drawPlantsDepthmap()
{
  if (options.get(OPT_DRAW_TREES))
    {
      shaderManager.get(SHADER_SHADOW_MODELS).use();
      {
        BENCHMARK("Renderer: draw models depthmap", true);
        renderer.renderPlants(plantGeneratorFacade, shaderManager.get(SHADER_SHADOW_MODELS),
                           false,
                           false,
                           false);
      }
    }
}

const std::shared_ptr<WaterGenerator> WorldGeneratorFacade::getWaterGenerator() const
{
  return waterGenerator;
}

const std::unique_ptr<HillsFacade>& WorldGeneratorFacade::getHillsFacade() const
{
  return hillsFacade;
}

const std::shared_ptr<PlantGeneratorFacade> WorldGeneratorFacade::getPlantsGeneratorFacade() const
{
  return plantGeneratorFacade;
}
