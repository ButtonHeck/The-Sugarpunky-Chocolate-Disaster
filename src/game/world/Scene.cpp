#include "game/world/Scene.h"

Scene::Scene(ShaderManager &shaderManager, Options &options, TextureManager &textureManager, ScreenResolution &screenResolution)
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
                 shaderManager.get(SHADER_MODELS_GOURAUD)),
    skyboxFacade(shaderManager.get(SHADER_SKYBOX)),
    theSunFacade(shaderManager.get(SHADER_SUN)),
    underwaterFacade(shaderManager.get(SHADER_UNDERWATER)),
    landFacade(std::make_unique<LandFacade>(shaderManager.get(SHADER_LAND))),
    lensFlareFacade(shaderManager.get(SHADER_LENS_FLARE), textureManager.getLoader())
{
  float sunPointSizeDivisorX = screenResolution.getWidthRatioToReference();
  float sunPointSizeDivisorY = screenResolution.getHeightRatioToReference();
  float sunPointSizeDivisor = (sunPointSizeDivisorX + sunPointSizeDivisorY) / 2;
  float sunReflectionPointSizeDivisorX = FRAME_WATER_REFLECTION_WIDTH / ScreenResolution::REFERENCE_WIDTH;
  float sunReflectionPointSizeDivisorY = FRAME_WATER_REFLECTION_HEIGHT / ScreenResolution::REFERENCE_HEIGHT;
  float sunReflectionPointSizeDivisor = (sunReflectionPointSizeDivisorX + sunReflectionPointSizeDivisorY) / 2;
  theSunFacade.adjustSunPointSize(sunPointSizeDivisor, sunReflectionPointSizeDivisor);
  lensFlareFacade.adjustFlaresPointSize(sunPointSizeDivisor);
}

void Scene::setup()
{
  BENCHMARK("Scene: setup", false);
  waterFacade.setup();
  hillsFacade.setup();
  shoreFacade.setup();
  landFacade->setup(shoreFacade.getMap());
  waterFacade.setupConsiderTerrain();
  buildableFacade.setup(landFacade->getMap(), hillsFacade.getMap());
  plantsFacade.setup(landFacade->getMap(), hillsFacade.getMap(), hillsFacade.getNormalMap());
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
                      const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                      glm::mat4& projectionView,
                      glm::mat4& skyProjectionView,
                      Frustum &viewFrustum,
                      Frustum &cullingViewFrustum,
                      Camera& camera,
                      MouseInputManager& mouseInput)
{
  BENCHMARK("Scene: draw all", true);
  glm::vec3 viewPosition = camera.getPosition();
  glm::vec2 viewAcceleration = camera.getViewAcceleration();

  hillsFacade.draw(lightDir,
                   lightSpaceMatrices,
                   projectionView, viewPosition, viewAcceleration, cullingViewFrustum,
                   options[OPT_HILLS_CULLING],
                   options[OPT_USE_SHADOWS],
                   options[OPT_DEBUG_RENDER]);

  if (options[OPT_DRAW_LAND])
    landFacade->draw(lightDir,
                     lightSpaceMatrices,
                     projectionView, viewFrustum, options[OPT_USE_SHADOWS]);

  underwaterFacade.draw(lightDir, projectionView);

  shoreFacade.draw(lightDir,
                   lightSpaceMatrices,
                   projectionView, options[OPT_USE_SHADOWS], options[OPT_DEBUG_RENDER],
                   false, false);

  if (options[OPT_DRAW_TREES])
    plantsFacade.draw(lightDir,
                      lightSpaceMatrices,
                      projectionView, viewPosition,
                      options[OPT_MODELS_PHONG_SHADING],
                      options[OPT_USE_SHADOWS],
                      options[OPT_MODELS_LAND_BLENDING]);

  if (options[OPT_DRAW_BUILDABLE])
    buildableFacade.drawBuildable(projectionView);

  if (options[OPT_SHOW_CURSOR])
    {
      mouseInput.updateCursorMappingCoordinates(camera, landFacade->getMap(), hillsFacade.getMap(), buildableFacade.getMap());
      buildableFacade.drawSelected(projectionView, mouseInput);
    }

  RendererStateManager::setAmbienceRenderingState(true);
  theSunFacade.draw(skyProjectionView, true, false);
  skyboxFacade.draw(skyProjectionView, viewPosition, lightDir);
  RendererStateManager::setAmbienceRenderingState(false);

  if (options[OPT_DRAW_WATER])
    waterFacade.draw(lightDir,
                     lightSpaceMatrices,
                     projectionView, viewPosition, viewFrustum,
                     options[OPT_WATER_CULLING], options[OPT_DEBUG_RENDER]);

  float theSunVisibility = theSunFacade.getSunVisibilityPercentage();
  theSunVisibility *= glm::clamp(-(lightDir.y + 0.02f - camera.getPosition().y / 1500.0f) * 8.0f, 0.0f, 1.0f);
  if (theSunVisibility > 0)
    lensFlareFacade.draw(theSunFacade.getCurrentPosition(), skyProjectionView, theSunVisibility);
}

void Scene::drawWorldDepthmap(const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                              bool grassCastShadow)
{
  BENCHMARK("Scene: draw depthmap all", true);
  glClear(GL_DEPTH_BUFFER_BIT);

  glDisable(GL_MULTISAMPLE);
  shaderManager.get(SHADER_SHADOW_TERRAIN).use();
  shaderManager.get(SHADER_SHADOW_TERRAIN).setMat4("u_lightSpaceMatrix[0]", lightSpaceMatrices[0]);
  shaderManager.get(SHADER_SHADOW_TERRAIN).setMat4("u_lightSpaceMatrix[1]", lightSpaceMatrices[1]);
  shaderManager.get(SHADER_SHADOW_TERRAIN).setMat4("u_lightSpaceMatrix[2]", lightSpaceMatrices[2]);
  hillsFacade.drawDepthmap();
  if (options[OPT_DRAW_TREES])
    {
      shaderManager.get(SHADER_SHADOW_MODELS).use();
      shaderManager.get(SHADER_SHADOW_MODELS).setMat4("u_lightSpaceMatrix[0]", lightSpaceMatrices[0]);
      shaderManager.get(SHADER_SHADOW_MODELS).setMat4("u_lightSpaceMatrix[1]", lightSpaceMatrices[1]);
      shaderManager.get(SHADER_SHADOW_MODELS).setMat4("u_lightSpaceMatrix[2]", lightSpaceMatrices[2]);
      plantsFacade.drawDepthmap(grassCastShadow);
    }
  glEnable(GL_MULTISAMPLE);
}

void Scene::drawWorldReflection(glm::vec3 lightDir,
                                const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                                glm::mat4 &projectionView,
                                glm::mat4 &skyProjectionView,
                                Frustum &cullingViewFrustum,
                                Camera &camera)
{
  glm::vec3 viewPosition = camera.getPosition();
  viewPosition.y *= -1;
  glm::vec2 viewAcceleration = camera.getViewAcceleration();

  hillsFacade.draw(lightDir,
                   lightSpaceMatrices,
                   projectionView, viewPosition, viewAcceleration, cullingViewFrustum,
                   options[OPT_HILLS_CULLING],
                   options[OPT_USE_SHADOWS],
                   false);

  glEnable(GL_CLIP_DISTANCE0);
  shoreFacade.draw(lightDir,
                   lightSpaceMatrices,
                   projectionView, false, false,
                   true, false);
  glDisable(GL_CLIP_DISTANCE0);

  if (options[OPT_DRAW_TREES])
    plantsFacade.draw(lightDir,
                      lightSpaceMatrices,
                      projectionView, viewPosition,
                      false,
                      false,
                      false);

  RendererStateManager::setAmbienceRenderingState(true);
  theSunFacade.draw(skyProjectionView, false, true);
  skyboxFacade.draw(skyProjectionView, viewPosition, lightDir);
  RendererStateManager::setAmbienceRenderingState(false);
}

void Scene::drawWorldRefraction(glm::vec3 lightDir,
                                const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                                glm::mat4 &projectionView)
{
  underwaterFacade.draw(lightDir, projectionView);

  glEnable(GL_CLIP_DISTANCE0);
  shoreFacade.draw(lightDir,
                   lightSpaceMatrices,
                   projectionView, false, false,
                   false, true);
  glDisable(GL_CLIP_DISTANCE0);
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
