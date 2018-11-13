#include "game/world/Scene.h"

Scene::Scene(ShaderManager &shaderManager, Options &options, TextureManager &textureManager)
  :
    shaderManager(shaderManager),
    options(options),
    textureManager(textureManager),
    waterFacade(shaderManager.get(SHADER_WATER),
                shaderManager.get(SHADER_WATER_CULLING),
                shaderManager.get(SHADER_WATER_NORMALS)),
    hillsFacade(shaderManager.get(SHADER_HILLS),
                shaderManager.get(SHADER_HILLS_CULLING),
                shaderManager.get(SHADER_HILLS_NORMALS),
                waterFacade.getMap()),
    shoreFacade(shaderManager.get(SHADER_SHORE),
                shaderManager.get(SHADER_SHORE_NORMALS),
                waterFacade.getMap()),
    buildableFacade(shaderManager.get(SHADER_BUILDABLE),
                    shaderManager.get(SHADER_SELECTED)),
    plantsFacade(shaderManager.get(SHADER_MODELS_PHONG),
                 shaderManager.get(SHADER_MODELS)),
    skyboxFacade(shaderManager.get(SHADER_SKYBOX)),
    theSunFacade(shaderManager.get(SHADER_SUN)),
    underwaterFacade(shaderManager.get(SHADER_UNDERWATER)),
    landFacade(std::make_unique<LandFacade>(shaderManager.get(SHADER_LAND)))
{}

void Scene::setup()
{
  BENCHMARK("Scene: setup", false);
  waterFacade.setup();
  hillsFacade.setup();
  shoreFacade.setup();
  landFacade->setup(shoreFacade.getMap());
  waterFacade.setupConsiderTerrain();
  buildableFacade.setup(landFacade->getMap(), hillsFacade.getMap());
  plantsFacade.setup(landFacade->getMap(), hillsFacade.getMap());
  textureManager.createUnderwaterReliefTexture(waterFacade.getMap());
}

void Scene::recreate()
{
  landFacade.reset(new LandFacade(shaderManager.get(SHADER_LAND)));
  Generator::initializeMap(const_cast<map2D_f&>(waterFacade.getMap()));
  Generator::initializeMap(const_cast<map2D_f&>(hillsFacade.getMap()));
  setup();
}

void Scene::load()
{
  hillsFacade.createTilesAndBufferData();
  shoreFacade.setup();
  landFacade->setup(shoreFacade.getMap());
  waterFacade.setupConsiderTerrain();
  buildableFacade.setup(landFacade->getMap(), hillsFacade.getMap());
  textureManager.createUnderwaterReliefTexture(waterFacade.getMap());
}

void Scene::serialize(std::ofstream &output)
{
  landFacade->serialize(output);
  shoreFacade.serialize(output);
  hillsFacade.serialize(output);
  waterFacade.serialize(output);
  plantsFacade.serialize(output);
}

void Scene::deserialize(std::ifstream &input)
{
  landFacade->deserialize(input);
  shoreFacade.deserialize(input);
  hillsFacade.deserialize(input);
  waterFacade.deserialize(input);
  plantsFacade.deserialize(input);
}

void Scene::drawWorld(glm::vec3 lightDir,
                      glm::mat4 lightSpaceMatrix,
                      glm::mat4& projectionView,
                      glm::mat4& skyProjectionView,
                      Frustum &viewFrustum,
                      Camera& camera,
                      MouseInputManager& mouseInput)
{
  BENCHMARK("Scene: draw all", true);
  glm::vec3 viewPosition = camera.getPosition();

  hillsFacade.draw(lightDir, lightSpaceMatrix,
                   projectionView, viewPosition, viewFrustum,
                   options[OPT_HILLS_CULLING],
                   options[OPT_USE_SHADOWS],
                   options[OPT_DEBUG_RENDER]);

  if (options[OPT_DRAW_LAND])
    landFacade->draw(lightDir, lightSpaceMatrix,
                     projectionView, viewFrustum, options[OPT_USE_SHADOWS]);

  underwaterFacade.draw(lightDir, projectionView);

  shoreFacade.draw(lightDir, lightSpaceMatrix,
                   projectionView,
                   options[OPT_USE_SHADOWS], options[OPT_DEBUG_RENDER]);

  if (options[OPT_DRAW_TREES])
    plantsFacade.draw(lightDir, lightSpaceMatrix,
                      projectionView, viewPosition,
                      options[OPT_MODELS_PHONG_SHADING],
                      options[OPT_MODELS_SHADOW_EMPHASIZE],
                      options[OPT_USE_SHADOWS],
                      options[OPT_MODELS_FLAT_BLENDING]);

  if (options[OPT_DRAW_BUILDABLE])
    buildableFacade.drawBuildable(projectionView);

  if (options[OPT_SHOW_CURSOR])
    {
      mouseInput.updateCursorMappingCoordinates(camera, landFacade->getMap(), hillsFacade.getMap(), buildableFacade.getMap());
      buildableFacade.drawSelected(projectionView, mouseInput);
    }

  theSunFacade.draw(skyProjectionView);

  skyboxFacade.draw(skyProjectionView, viewPosition);

  if (options[OPT_DRAW_WATER])
    waterFacade.draw(lightDir, projectionView, viewPosition, viewFrustum,
                     options[OPT_WATER_CULLING], options[OPT_DEBUG_RENDER]);
}

void Scene::drawWorldDepthmap(glm::mat4 lightSpaceMatrix)
{
  BENCHMARK("Scene: draw depthmap all", true);
  glClear(GL_DEPTH_BUFFER_BIT);

  glDisable(GL_CULL_FACE); //or set front face culling
  shaderManager.get(SHADER_SHADOW_TERRAIN).use();
  shaderManager.get(SHADER_SHADOW_TERRAIN).setMat4("u_lightSpaceMatrix", lightSpaceMatrix);
  hillsFacade.drawDepthmap();
  shoreFacade.drawDepthmap();
  underwaterFacade.drawDepthmap();
  if (options[OPT_DRAW_TREES])
    {
      shaderManager.get(SHADER_SHADOW_MODELS).use();
      shaderManager.get(SHADER_SHADOW_MODELS).setMat4("u_lightSpaceMatrix", lightSpaceMatrix);
      plantsFacade.drawDepthmap();
    }
  glEnable(GL_CULL_FACE); //or set back face culling
}

WaterFacade &Scene::getWaterFacade()
{
  return waterFacade;
}

HillsFacade &Scene::getHillsFacade()
{
  return hillsFacade;
}

PlantsFacade &Scene::getPlantsFacade()
{
  return plantsFacade;
}

TheSunFacade &Scene::getSunFacade()
{
  return theSunFacade;
}
