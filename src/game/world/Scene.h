#ifndef SCENE_H
#define SCENE_H
#include <memory>
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

class Scene
{
public:
  Scene(ShaderManager& shaderManager, Options& options, TextureManager& textureManager, ScreenResolution& screenResolution);

  //internal generators functions
  void setup();
  void recreate();
  void load();
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);

  //rendering part
  void drawWorld(const glm::vec3 &lightDir,
                 const std::array<glm::mat4, NUM_SHADOW_LAYERS>& lightSpaceMatrices,
                 const glm::mat4 &projectionView,
                 const glm::mat4 &skyProjectionView,
                 const Frustum &viewFrustum,
                 const Frustum &cullingViewFrustum,
                 const Camera &camera,
                 MouseInputManager& mouseInput);
  void drawWorldDepthmap(const std::array<glm::mat4, NUM_SHADOW_LAYERS>& lightSpaceMatrices,
                         bool grassCastShadow);
  void drawWorldReflection(const glm::vec3 &lightDir,
                           const std::array<glm::mat4, NUM_SHADOW_LAYERS>& lightSpaceMatrices,
                           const glm::mat4 &projectionView,
                           const glm::mat4 &skyProjectionView,
                           const Frustum &cullingViewFrustum,
                           const Camera &camera);
  void drawWorldRefraction(const glm::vec3 &lightDir,
                           const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                           const glm::mat4 &projectionView);

  //getters
  WaterFacade &getWaterFacade();
  HillsFacade &getHillsFacade();
  PlantsFacade &getPlantsFacade();
  TheSunFacade &getSunFacade();
  SkysphereFacade &getSkysphereFacade();
  std::unique_ptr<LandFacade> &getLandFacade();

private:
  ShaderManager& shaderManager;
  Options& options;
  TextureManager& textureManager;

  WaterFacade waterFacade;
  HillsFacade hillsFacade;
  ShoreFacade shoreFacade;
  BuildableFacade buildableFacade;
  PlantsFacade plantsFacade;
  SkyboxFacade skyboxFacade;
  TheSunFacade theSunFacade;
  UnderwaterFacade underwaterFacade;
  std::unique_ptr<LandFacade> landFacade;
  LensFlareFacade lensFlareFacade;
  SkysphereFacade skysphereFacade;
};

#endif // SCENE_H
