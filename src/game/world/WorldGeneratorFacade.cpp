#include "game/world/WorldGeneratorFacade.h"

WorldGeneratorFacade::WorldGeneratorFacade(ShaderManager &shaderManager, Renderer &renderer, Options &options, TextureManager &textureManager)
  :
    shaderManager(shaderManager),
    renderer(renderer),
    options(options),
    textureManager(textureManager)
{
  waterGenerator = std::make_shared<WaterGenerator>(shaderManager.get(SHADER_WATER_CULLING));
  hillsGenerator = std::make_shared<HillsGenerator>(shaderManager.get(SHADER_HILLS_CULLING), waterGenerator->getMap());
  landGenerator = std::make_shared<LandGenerator>();
  shoreGenerator = std::make_shared<ShoreGenerator>(waterGenerator->getMap());
  buildableGenerator = std::make_shared<BuildableGenerator>(landGenerator, hillsGenerator);
  plantGeneratorFacade = std::make_shared<PlantGeneratorFacade>();
}

void WorldGeneratorFacade::setup()
{
  waterGenerator->setup();
  hillsGenerator->setup();
  shoreGenerator->setup();
  landGenerator->setup(shoreGenerator->getMap());
  waterGenerator->setupConsiderTerrain();
  buildableGenerator->setup(landGenerator, hillsGenerator);
  plantGeneratorFacade->setup(landGenerator->getMap(), hillsGenerator->getMap());
  textureManager.createUnderwaterReliefTexture(waterGenerator);
}

void WorldGeneratorFacade::recreate()
{
  landGenerator.reset(new LandGenerator());
  initializeMap(waterGenerator->getMap());
  initializeMap(hillsGenerator->getMap());
  setup();
}

void WorldGeneratorFacade::load()
{
  hillsGenerator->createTilesAndBufferData();
  shoreGenerator->setup();
  landGenerator->setup(shoreGenerator->getMap());
  waterGenerator->setupConsiderTerrain();
  buildableGenerator->setup(landGenerator, hillsGenerator);
  textureManager.createUnderwaterReliefTexture(waterGenerator);
}

void WorldGeneratorFacade::serialize(std::ofstream &output)
{
  landGenerator->serialize(output);
  shoreGenerator->serialize(output);
  hillsGenerator->serialize(output);
  waterGenerator->serialize(output);
  plantGeneratorFacade->serialize(output);
}

void WorldGeneratorFacade::deserialize(std::ifstream &input)
{
  landGenerator->deserialize(input);
  shoreGenerator->deserialize(input);
  hillsGenerator->deserialize(input);
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
  drawHills(viewPosition, viewFrustum);
  drawFlatTerrain(viewFrustum);
  drawSkybox(skyProjectionView, camera.getPosition().y);
  drawUnderwater();
  drawShore();
  drawPlants(viewPosition);
  drawBuildable();
  drawSelected(mouseInput, camera);
  drawWater(viewPosition, viewFrustum);
}

void WorldGeneratorFacade::drawWorldDepthmap()
{
  glClear(GL_DEPTH_BUFFER_BIT);
  glDisable(GL_CULL_FACE); //or set front face culling
  drawTerrainDepthmap();
  drawPlantsDepthmap();
  glEnable(GL_CULL_FACE); //or set back face culling
}

void WorldGeneratorFacade::drawHills(glm::vec3 &viewPosition, Frustum &viewFrustum)
{
  shaderManager.updateHillsShaders(options.get(OPT_HILLS_CULLING), options.get(OPT_USE_SHADOWS), projectionView, viewPosition, viewFrustum, hillsGenerator->getMaxHeight());
  {
    BENCHMARK("Renderer: draw hills", true);
    renderer.renderHills(options.get(OPT_HILLS_CULLING), hillsGenerator, shaderManager.get(SHADER_HILLS_CULLING), shaderManager.get(SHADER_HILLS));
  }
}

void WorldGeneratorFacade::drawFlatTerrain(Frustum &viewFrustum)
{
  if (options.get(OPT_DRAW_FLAT_TERRAIN))
    {
      shaderManager.updateFlatShader(projectionView, options.get(OPT_USE_SHADOWS));
      {
        BENCHMARK("Renderer: draw flat", true);
        renderer.renderLand(landGenerator, viewFrustum, textureManager.get(TEX_LAND));
      }
    }
}

void WorldGeneratorFacade::drawUnderwater()
{
  shaderManager.updateUnderwaterShader(projectionView);
  renderer.renderUnderwater(underwaterGenerator);
}

void WorldGeneratorFacade::drawShore()
{
  shaderManager.updateShoreShader(projectionView, options.get(OPT_USE_SHADOWS));
  {
    BENCHMARK("Renderer: draw shore", true);
    renderer.renderShore(shoreGenerator);
  }
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

void WorldGeneratorFacade::drawBuildable()
{
  if (options.get(OPT_DRAW_BUILDABLE))
    {
      shaderManager.updateBuildableShader(projectionView);
      {
        BENCHMARK("Renderer: draw buildable", true);
        renderer.renderBuildableTiles(buildableGenerator);
      }
    }
}

void WorldGeneratorFacade::drawSelected(MouseInputManager& mouseInput, Camera& camera)
{
  if (options.get(OPT_SHOW_CURSOR))
    {
      mouseInput.updateCursorMappingCoordinates(camera,
                                                landGenerator,
                                                hillsGenerator,
                                                buildableGenerator);
      if (buildableGenerator->getMap()[mouseInput.getCursorMapZ()][mouseInput.getCursorMapX()] != 0)
        {
          glm::mat4 selectedModel;
          selectedModel = glm::translate(selectedModel, glm::vec3(-HALF_WORLD_WIDTH + mouseInput.getCursorMapX(), 0.0f, -HALF_WORLD_HEIGHT + mouseInput.getCursorMapZ()));
          shaderManager.updateSelectedShader(projectionView, selectedModel);
          renderer.renderSelectedTile(buildableGenerator);
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

void WorldGeneratorFacade::drawSkybox(glm::mat4& skyProjectionView, float cameraY)
{
  glDepthMask(GL_FALSE);
  shaderManager.updateSkyShader(skyProjectionView, cameraY, 0);
  renderer.renderSkybox(&skybox);
  shaderManager.updateSkyShader(skyProjectionView, cameraY, 1);
  renderer.renderSkybox(&skybox);
  glDepthMask(GL_TRUE);
}

void WorldGeneratorFacade::drawTerrainDepthmap()
{
  shaderManager.get(SHADER_SHADOW_TERRAIN).use();
  renderer.renderHillsDepthmap(hillsGenerator);
  renderer.renderShore(shoreGenerator);
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

const std::shared_ptr<HillsGenerator> WorldGeneratorFacade::getHillsGenerator() const
{
  return hillsGenerator;
}

const std::shared_ptr<PlantGeneratorFacade> WorldGeneratorFacade::getPlantsGeneratorFacade() const
{
  return plantGeneratorFacade;
}
