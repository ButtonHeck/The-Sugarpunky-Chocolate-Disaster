#ifndef SCENE_H
#define SCENE_H
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
#include "ShaderManager"
#include "TextureManager"
#include "MouseInputManager"
#include "ScreenResolution"
#include "ShadowVolume"

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
  WaterFacade &getWaterFacade();
  HillsFacade &getHillsFacade();
  PlantsFacade &getPlantsFacade();
  TheSunFacade &getSunFacade();
  SkysphereFacade &getSkysphereFacade();
  LandFacade &getLandFacade();

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

#endif // SCENE_H
