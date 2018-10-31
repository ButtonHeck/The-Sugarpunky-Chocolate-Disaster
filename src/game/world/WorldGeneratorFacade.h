#ifndef TERRAINGENERATORFACADE_H
#define TERRAINGENERATORFACADE_H
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
#include "graphics/ShaderManager.h"
#include "graphics/TextureManager.h"
#include "input/MouseInputManager.h"

class WorldGeneratorFacade
{
public:
  WorldGeneratorFacade(ShaderManager& shaderManager, Options& options, TextureManager& textureManager);

  //internal generators functions
  void setup();
  void recreate();
  void load();
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);

  //rendering part
  void drawWorld(glm::mat4& projectionView,
                 glm::mat4& skyProjectionView,
                 Frustum &viewFrustum,
                 Camera& camera,
                 MouseInputManager& mouseInput);
  void drawWorldDepthmap();

  //getters
  const std::unique_ptr<WaterFacade> &getWaterFacade() const;
  const std::unique_ptr<HillsFacade> &getHillsFacade() const;
  const std::unique_ptr<PlantsFacade> &getPlantsFacade() const;

private:
  //internals
  ShaderManager& shaderManager;
  Options& options;
  TextureManager& textureManager;
  glm::mat4 projectionView;
  std::unique_ptr<WaterFacade> waterFacade;
  std::unique_ptr<HillsFacade> hillsFacade;
  std::unique_ptr<LandFacade> landFacade;
  std::unique_ptr<ShoreFacade> shoreFacade;
  std::unique_ptr<BuildableFacade> buildableFacade;
  std::unique_ptr<PlantsFacade> plantsFacade;
  std::unique_ptr<SkyboxFacade> skyboxFacade;
  std::unique_ptr<TheSunFacade> theSunFacade;
  std::unique_ptr<UnderwaterFacade> underwaterFacade;
};

#endif // TERRAINGENERATORFACADE_H
