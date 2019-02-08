#ifndef SCENE_H
#define SCENE_H
#include "game/world/terrain/land/LandFacade.h"
#include "game/world/terrain/hills/HillsFacade.h"
#include "game/world/models/plants/PlantsFacade.h"
#include "game/world/ambience/skysphere/SkysphereFacade.h"
#include "game/world/terrain/water/WaterFacade.h"
#include "game/world/terrain/buildable/BuildableFacade.h"
#include "game/world/terrain/shore/ShoreFacade.h"
#include "game/world/terrain/underwater/UnderwaterFacade.h"
#include "game/world/ambience/skybox/SkyboxFacade.h"
#include "game/world/ambience/theSun/TheSunFacade.h"
#include "game/world/ambience/lensFlare/LensFlareFacade.h"
#include "graphics/shaders/ShaderManager.h"
#include "graphics/textures/TextureManager.h"
#include "input/MouseInputManager.h"
#include "graphics/ScreenResolution.h"
#include "graphics/ShadowVolume.h"

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
