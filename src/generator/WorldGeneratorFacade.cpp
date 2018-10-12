#include "WorldGeneratorFacade.h"

WorldGeneratorFacade::WorldGeneratorFacade(ShaderManager &shaderManager, Renderer &renderer, Options &options)
  :
    shaderManager(shaderManager),
    renderer(renderer),
    options(options)
{
  waterMapGenerator = std::make_shared<WaterMapGenerator>(shaderManager.get(SHADER_WATER_CULLING));
  hillMapGenerator = std::make_shared<HillsMapGenerator>(shaderManager.get(SHADER_HILLS_CULLING), waterMapGenerator->getMap());
  baseMapGenerator = std::make_shared<BaseMapGenerator>();
  shoreGenerator = std::make_shared<ShoreGenerator>(waterMapGenerator->getMap());
  buildableMapGenerator = std::make_shared<BuildableMapGenerator>(baseMapGenerator, hillMapGenerator);
  plantGeneratorFacade = std::make_shared<PlantGeneratorFacade>();
}

void WorldGeneratorFacade::prepareTerrain()
{
  waterMapGenerator->setup();
  hillMapGenerator->setup();
  shoreGenerator->setup();
  baseMapGenerator->setup(shoreGenerator->getMap());
  waterMapGenerator->setupConsiderTerrain();
  buildableMapGenerator->setup(baseMapGenerator, hillMapGenerator);
  plantGeneratorFacade->setup(baseMapGenerator->getMap(), hillMapGenerator->getMap());
}

void WorldGeneratorFacade::recreate()
{
  baseMapGenerator.reset(new BaseMapGenerator());
  initializeMap(waterMapGenerator->getMap());
  initializeMap(hillMapGenerator->getMap());
  prepareTerrain();
}

void WorldGeneratorFacade::load()
{
  hillMapGenerator->createTilesAndBufferData();
  shoreGenerator->setup();
  baseMapGenerator->setup(shoreGenerator->getMap());
  waterMapGenerator->setupConsiderTerrain();
  buildableMapGenerator->setup(baseMapGenerator, hillMapGenerator);
}

void WorldGeneratorFacade::serialize(std::ofstream &output)
{
  baseMapGenerator->serialize(output);
  shoreGenerator->serialize(output);
  hillMapGenerator->serialize(output);
  waterMapGenerator->serialize(output);
  plantGeneratorFacade->serialize(output);
}

void WorldGeneratorFacade::deserialize(std::ifstream &input)
{
  baseMapGenerator->deserialize(input);
  shoreGenerator->deserialize(input);
  hillMapGenerator->deserialize(input);
  waterMapGenerator->deserialize(input);
  plantGeneratorFacade->deserialize(input);
}

void WorldGeneratorFacade::bufferWaterNewData()
{
  if (options.get(OPT_ANIMATE_WATER))
    {
      BENCHMARK("Water: buffer animation frame", true);
      waterMapGenerator->bufferNewData();
    }
}

void WorldGeneratorFacade::drawWorld(glm::mat4& projectionView,
                                     glm::vec3 &viewPosition,
                                     Frustum &viewFrustum,
                                     TextureManager& textureManager,
                                     Camera& camera,
                                     MouseInputManager& mouseInput,
                                     unsigned long updateCount)
{
  this->projectionView = projectionView;
  drawHills(viewPosition, viewFrustum);
  drawFlatTerrain(viewFrustum, textureManager);
  drawUnderwater();
  drawShore();
  drawPlants(viewPosition, updateCount);
  drawBuildable();
  drawSelected(mouseInput, camera);
  drawWater(viewPosition, viewFrustum);
}

void WorldGeneratorFacade::drawWorldDepthmap(unsigned long updateCount)
{
  glDisable(GL_CULL_FACE); //or set front face culling
  drawTerrainDepthmap();
  drawPlantsDepthmap(updateCount);
  glEnable(GL_CULL_FACE); //or set back face culling
}

void WorldGeneratorFacade::drawHills(glm::vec3 &viewPosition, Frustum &viewFrustum)
{
  shaderManager.updateHillsShaders(options.get(OPT_HILLS_CULLING), options.get(OPT_USE_SHADOWS), projectionView, viewPosition, viewFrustum, hillMapGenerator->getMaxHeight());
  {
    BENCHMARK("Renderer: draw hills", true);
    renderer.renderHills(options.get(OPT_HILLS_CULLING), hillMapGenerator, shaderManager.get(SHADER_HILLS_CULLING), shaderManager.get(SHADER_HILLS));
  }
}

void WorldGeneratorFacade::drawFlatTerrain(Frustum &viewFrustum, TextureManager& textureManager)
{
  if (options.get(OPT_DRAW_FLAT_TERRAIN))
    {
      shaderManager.updateFlatShader(projectionView, options.get(OPT_USE_SHADOWS));
      {
        BENCHMARK("Renderer: draw flat", true);
        renderer.renderFlatTerrain(baseMapGenerator, viewFrustum, textureManager.get(TEX_FLAT));
      }
    }
}

void WorldGeneratorFacade::drawUnderwater()
{
  shaderManager.updateUnderwaterShader(projectionView);
  renderer.renderUnderwater(underwaterQuadGenerator);
}

void WorldGeneratorFacade::drawShore()
{
  shaderManager.updateShoreShader(projectionView, options.get(OPT_USE_SHADOWS));
  {
    BENCHMARK("Renderer: draw shore", true);
    renderer.renderShore(shoreGenerator);
  }
}

void WorldGeneratorFacade::drawPlants(glm::vec3& viewPosition, unsigned long updateCount)
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
                           options.get(OPT_MODELS_CULLING),
                           true,
                           updateCount % MESH_INDIRECT_BUFFER_UPDATE_FREQ == 0,
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
        renderer.renderBuildableTiles(buildableMapGenerator);
      }
    }
}

void WorldGeneratorFacade::drawSelected(MouseInputManager& mouseInput, Camera& camera)
{
  if (options.get(OPT_SHOW_CURSOR))
    {
      mouseInput.updateCursorMappingCoordinates(camera,
                                                baseMapGenerator,
                                                hillMapGenerator,
                                                buildableMapGenerator);
      if (buildableMapGenerator->getMap()[mouseInput.getCursorMapZ()][mouseInput.getCursorMapX()] != 0)
        {
          glm::mat4 selectedModel;
          selectedModel = glm::translate(selectedModel, glm::vec3(-HALF_WORLD_WIDTH + mouseInput.getCursorMapX(), 0.0f, -HALF_WORLD_HEIGHT + mouseInput.getCursorMapZ()));
          shaderManager.updateSelectedShader(projectionView, selectedModel);
          renderer.renderSelectedTile(buildableMapGenerator);
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
        renderer.renderWater(options.get(OPT_WATER_CULLING), waterMapGenerator, shaderManager.get(SHADER_WATER_CULLING), shaderManager.get(SHADER_WATER));
      }
    }
}

void WorldGeneratorFacade::drawTerrainDepthmap()
{
  shaderManager.get(SHADER_SHADOW_TERRAIN).use();
  renderer.renderHillsDepthmap(hillMapGenerator);
  renderer.renderShore(shoreGenerator);
}

void WorldGeneratorFacade::drawPlantsDepthmap(unsigned long updateCount)
{
  if (options.get(OPT_DRAW_TREES))
    {
      shaderManager.get(SHADER_SHADOW_MODELS).use();
      {
        BENCHMARK("Renderer: draw models depthmap", true);
        renderer.renderPlants(plantGeneratorFacade, shaderManager.get(SHADER_SHADOW_MODELS),
                           options.get(OPT_MODELS_CULLING),
                           false,
                           updateCount % MESH_INDIRECT_BUFFER_UPDATE_FREQ == 0,
                           false,
                           false);
      }
    }
}

const std::shared_ptr<WaterMapGenerator> WorldGeneratorFacade::getWaterGenerator() const
{
  return waterMapGenerator;
}

const std::shared_ptr<HillsMapGenerator> WorldGeneratorFacade::getHillsGenerator() const
{
  return hillMapGenerator;
}

const std::shared_ptr<PlantGeneratorFacade> WorldGeneratorFacade::getPlantsGeneratorFacade() const
{
  return plantGeneratorFacade;
}
