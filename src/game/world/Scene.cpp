#include "Scene"

Scene::Scene(ShaderManager &shaderManager, Options &options, TextureManager &textureManager, const ScreenResolution &screenResolution, const ShadowVolume &shadowVolume)
  :
    shaderManager(shaderManager),
    options(options),
    textureManager(textureManager),
    shadowVolume(shadowVolume),
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
    theSunFacade(shaderManager.get(SHADER_SUN), screenResolution),
    underwaterFacade(shaderManager.get(SHADER_UNDERWATER)),
    landFacade(shaderManager.get(SHADER_LAND)),
    lensFlareFacade(shaderManager.get(SHADER_LENS_FLARE), textureManager.getLoader(), screenResolution),
    skysphereFacade(shaderManager.get(SHADER_SKYSPHERE))
{}

void Scene::setup()
{
  BENCHMARK("Scene: setup", false);
  waterFacade.setup();
  hillsFacade.setup();
  shoreFacade.setup();
  landFacade.setup(shoreFacade.getMap());
  waterFacade.setupConsiderTerrain();
  buildableFacade.setup(landFacade.getMap(), hillsFacade.getMap());
  plantsFacade.setup(landFacade.getMap(), hillsFacade.getMap(), hillsFacade.getNormalMap());
  textureManager.createUnderwaterReliefTexture(waterFacade.getMap());
}

void Scene::recreate()
{
  Generator::initializeMap(const_cast<map2D_f&>(landFacade.getMap()));
  Generator::initializeMap(const_cast<map2D_f&>(waterFacade.getMap()));
  Generator::initializeMap(const_cast<map2D_f&>(hillsFacade.getMap()));
  setup();
}

void Scene::load()
{
  hillsFacade.createTilesAndBufferData();
  shoreFacade.setup();
  landFacade.setup(shoreFacade.getMap());
  waterFacade.setupConsiderTerrain();
  buildableFacade.setup(landFacade.getMap(), hillsFacade.getMap());
  textureManager.createUnderwaterReliefTexture(waterFacade.getMap());
  plantsFacade.initializeModelRenderChunks(landFacade.getMap(), hillsFacade.getMap());
}

void Scene::serialize(std::ofstream &output)
{
  landFacade.serialize(output);
  hillsFacade.serialize(output);
  waterFacade.serialize(output);
  plantsFacade.serialize(output);
}

void Scene::deserialize(std::ifstream &input)
{
  landFacade.deserialize(input);
  hillsFacade.deserialize(input);
  waterFacade.deserialize(input);
  plantsFacade.deserialize(input);
}

void Scene::drawWorld(const glm::mat4& projectionView,
                      const glm::mat4& skyboxProjectionView,
                      const Frustum &viewFrustum,
                      const Frustum &cullingViewFrustum,
                      const Camera& camera,
                      MouseInputManager& mouseInput)
{
  BENCHMARK("Scene: draw all", true);
  glm::vec3 viewPosition = camera.getPosition();
  const glm::vec3& lightDir = theSunFacade.getLightDir();
  const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices = shadowVolume.getLightSpaceMatrices();
  bool useShadows = options[OPT_USE_SHADOWS];
  bool isDebugRender = options[OPT_DEBUG_RENDER];

  hillsFacade.draw(lightDir,
                   lightSpaceMatrices,
                   projectionView,
                   viewPosition,
                   camera.getViewAcceleration(),
                   cullingViewFrustum,
                   options[OPT_HILLS_CULLING],
                   useShadows,
                   isDebugRender);

  if (options[OPT_DRAW_LAND])
    landFacade.draw(lightDir, lightSpaceMatrices, projectionView, useShadows);

  underwaterFacade.draw(lightDir, projectionView);

  shoreFacade.draw(lightDir, lightSpaceMatrices, projectionView, useShadows, isDebugRender, false, false);

  if (options[OPT_DRAW_TREES])
    plantsFacade.draw(lightDir,
                      lightSpaceMatrices,
                      projectionView,
                      viewPosition,
                      options[OPT_MODELS_PHONG_SHADING],
                      useShadows,
                      options[OPT_MODELS_LAND_BLENDING]);

  if (options[OPT_DRAW_BUILDABLE])
    buildableFacade.drawBuildable(projectionView);

  if (options[OPT_SHOW_CURSOR])
    {
      mouseInput.updateCursorMappingCoordinates(camera, landFacade.getMap(), hillsFacade.getMap(), buildableFacade.getMap());
      buildableFacade.drawSelected(projectionView, mouseInput);
    }

  RendererStateManager::setAmbienceRenderingState(true);
  skysphereFacade.draw(theSunFacade.getRotationTransform(), skyboxProjectionView, lightDir);
  theSunFacade.draw(skyboxProjectionView, true, false);
  skyboxFacade.draw(skyboxProjectionView, viewPosition, lightDir);
  RendererStateManager::setAmbienceRenderingState(false);

  if (options[OPT_DRAW_WATER])
    waterFacade.draw(lightDir, lightSpaceMatrices, projectionView, viewPosition, viewFrustum, options[OPT_WATER_CULLING], isDebugRender);

  float flareBrightness = theSunFacade.getSunFlareBrightness(options[OPT_USE_MULTISAMPLING], viewPosition.y);
  if (flareBrightness > 0)
    lensFlareFacade.draw(theSunFacade.getPosition(), skyboxProjectionView, flareBrightness);
}

void Scene::drawWorldDepthmap(bool grassCastShadow)
{
  BENCHMARK("Scene: draw world depthmap", true);
  const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices = shadowVolume.getLightSpaceMatrices();
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

void Scene::drawWorldReflection(const glm::mat4 &projectionView,
                                const glm::mat4 &skyboxProjectionView,
                                const Frustum &cullingViewFrustum,
                                const Camera &camera)
{
  BENCHMARK("Scene: draw world reflection", true);
  glm::vec3 viewPosition = camera.getPosition();
  const glm::vec3& lightDir = theSunFacade.getLightDir();
  const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices = shadowVolume.getLightSpaceMatrices();
  viewPosition.y *= -1;

  hillsFacade.draw(lightDir,
                   lightSpaceMatrices,
                   projectionView,
                   viewPosition,
                   camera.getViewAcceleration(),
                   cullingViewFrustum,
                   options[OPT_HILLS_CULLING],
                   options[OPT_USE_SHADOWS],
                   false);

  glEnable(GL_CLIP_DISTANCE0);
  shoreFacade.draw(lightDir, lightSpaceMatrices, projectionView, false, false, true, false);
  glDisable(GL_CLIP_DISTANCE0);

  if (options[OPT_DRAW_TREES])
    plantsFacade.draw(lightDir, lightSpaceMatrices, projectionView, viewPosition, false, false, false);

  RendererStateManager::setAmbienceRenderingState(true);
  skysphereFacade.draw(theSunFacade.getRotationTransform(), skyboxProjectionView, lightDir);
  theSunFacade.draw(skyboxProjectionView, false, true);
  skyboxFacade.draw(skyboxProjectionView, viewPosition, lightDir);
  RendererStateManager::setAmbienceRenderingState(false);
}

void Scene::drawWorldRefraction(const glm::mat4 &projectionView)
{
  BENCHMARK("Scene: draw world refraction", true);
  const glm::vec3& lightDir = theSunFacade.getLightDir();
  const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices = shadowVolume.getLightSpaceMatrices();

  underwaterFacade.draw(lightDir, projectionView);

  glEnable(GL_CLIP_DISTANCE0);
  shoreFacade.draw(lightDir, lightSpaceMatrices, projectionView, false, false, false, true);
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

SkysphereFacade &Scene::getSkysphereFacade()
{
  return skysphereFacade;
}

LandFacade &Scene::getLandFacade()
{
  return landFacade;
}
