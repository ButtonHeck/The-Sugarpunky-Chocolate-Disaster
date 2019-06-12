#pragma once

#include "LandFacade"
#include "HillsFacade"
#include "PlantsFacade"
#include "SkysphereFacade"
#include "WaterFacade"
#include "BuildableFacade"
#include "ShoreFacade"
#include "UnderwaterFacade"
#include "SkyboxFacade"
#include "TheSunFacade"
#include "LensFlareFacade"

class ShaderManager;
class TextureManager;
class MouseInputManager;
class Camera;
class ScreenResolution;
class Options;
class ShadowVolume;

class Scene
{
public:
  Scene(ShaderManager& shaderManager, Options& options, TextureManager& textureManager, const ScreenResolution& screenResolution, const ShadowVolume& shadowVolume);

  //internal generators functions
  void setup();
  void recreate();
  void load();
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);

  //rendering part
  void drawWorld(const glm::mat4 &projectionView,
                 const glm::mat4 &skyProjectionView,
                 const Frustum &viewFrustum,
                 const Frustum &cullingViewFrustum,
                 const Camera &camera,
                 MouseInputManager& mouseInput);
  void drawWorldDepthmap(bool grassCastShadow);
  void drawWorldReflection(const glm::mat4 &projectionView,
                           const glm::mat4 &skyProjectionView,
                           const Frustum &cullingViewFrustum,
                           const Camera &camera);
  void drawWorldRefraction(const glm::mat4 &projectionView);

  //getters
  WaterFacade &getWaterFacade() noexcept;
  HillsFacade &getHillsFacade() noexcept;
  PlantsFacade &getPlantsFacade() noexcept;
  TheSunFacade &getSunFacade() noexcept;
  SkysphereFacade &getSkysphereFacade() noexcept;
  LandFacade &getLandFacade() noexcept;

private:
  ShaderManager& shaderManager;
  Options& options;
  TextureManager& textureManager;
  const ShadowVolume& shadowVolume;

  WaterFacade waterFacade;
  HillsFacade hillsFacade;
  ShoreFacade shoreFacade;
  BuildableFacade buildableFacade;
  PlantsFacade plantsFacade;
  SkyboxFacade skyboxFacade;
  TheSunFacade theSunFacade;
  UnderwaterFacade underwaterFacade;
  LandFacade landFacade;
  LensFlareFacade lensFlareFacade;
  SkysphereFacade skysphereFacade;
};
