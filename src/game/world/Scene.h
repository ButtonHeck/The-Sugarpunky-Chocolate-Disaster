#ifndef SCENE_H
#define SCENE_H
#include <memory>
#include "game/world/terrain/land/LandFacade.h"
#include "game/world/terrain/hills/HillsFacade.h"
#include "game/world/plants/PlantsFacade.h"
#include "game/world/terrain/water/WaterFacade.h"
#include "game/world/terrain/buildable/BuildableFacade.h"
#include "game/world/terrain/shore/ShoreFacade.h"
#include "game/world/terrain/underwater/UnderwaterFacade.h"
#include "game/world/ambience/skybox/SkyboxFacade.h"
#include "game/world/ambience/theSun/TheSunFacade.h"
#include "game/world/ambience/lensFlare/LensFlareFacade.h"
#include "graphics/ShaderManager.h"
#include "graphics/TextureManager.h"
#include "input/MouseInputManager.h"

class Scene
{
public:
  Scene(ShaderManager& shaderManager, Options& options, TextureManager& textureManager);

  //internal generators functions
  void setup();
  void recreate();
  void load();
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);

  //rendering part
  void drawWorld(glm::vec3 lightDir,
                 const std::array<glm::mat4, NUM_SHADOW_LAYERS>& lightSpaceMatrices,
                 glm::mat4& projectionView,
                 glm::mat4& skyProjectionView,
                 Frustum &viewFrustum,
                 Frustum &cullingViewFrustum,
                 Camera& camera,
                 MouseInputManager& mouseInput);
  void drawWorldDepthmap(const std::array<glm::mat4, NUM_SHADOW_LAYERS>& lightSpaceMatrices,
                         bool grassCastShadow);
  void drawWorldReflection(glm::vec3 lightDir,
                           const std::array<glm::mat4, NUM_SHADOW_LAYERS>& lightSpaceMatrices,
                           glm::mat4 &projectionView,
                           glm::mat4& skyProjectionView,
                           Frustum &cullingViewFrustum,
                           Camera& camera);
  void drawWorldRefraction(glm::vec3 lightDir,
                           const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                           glm::mat4 &projectionView);

  //getters
  WaterFacade &getWaterFacade();
  HillsFacade &getHillsFacade();
  PlantsFacade &getPlantsFacade();
  TheSunFacade &getSunFacade();

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
};

#endif // SCENE_H
