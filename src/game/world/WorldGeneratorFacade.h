#ifndef TERRAINGENERATORFACADE_H
#define TERRAINGENERATORFACADE_H
#include <memory>
#include "game/world/terrain/land/LandFacade.h"
#include "game/world/terrain/hills/HillsFacade.h"
#include "game/world/PlantGeneratorFacade.h"
#include "game/world/terrain/water/WaterFacade.h"
#include "game/world/terrain/buildable/BuildableFacade.h"
#include "game/world/terrain/shore/ShoreFacade.h"
#include "game/world/terrain/underwater/UnderwaterFacade.h"
#include "game/world/Skybox.h"
#include "game/world/TheSun.h"
#include "graphics/ShaderManager.h"
#include "graphics/Renderer.h"
#include "graphics/TextureManager.h"
#include "input/MouseInputManager.h"

class WorldGeneratorFacade
{
public:
  WorldGeneratorFacade(ShaderManager& shaderManager, Renderer& renderer, Options& options, TextureManager& textureManager);

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
  const std::unique_ptr<WaterFacade> &getWaterGenerator() const;
  const std::unique_ptr<HillsFacade> &getHillsFacade() const;
  const std::shared_ptr<PlantGeneratorFacade> getPlantsGeneratorFacade() const;

private:
  //rendering on screen
  void drawPlants(glm::vec3& viewPosition);
  void drawAmbient(glm::mat4& skyProjectionView, glm::vec3 &viewPosition);
  void drawSkybox(glm::mat4& skyProjectionView, glm::vec3 &viewPosition);
  void drawSun(glm::mat4& skyProjectionView);

  //rendering offscreen (depthmap)
  void drawTerrainDepthmap();
  void drawPlantsDepthmap();

  //internals
  ShaderManager& shaderManager;
  Renderer& renderer;
  Options& options;
  TextureManager& textureManager;
  glm::mat4 projectionView;
  std::unique_ptr<WaterFacade> waterFacade;
  std::unique_ptr<HillsFacade> hillsFacade;
  std::unique_ptr<LandFacade> landFacade;
  std::unique_ptr<ShoreFacade> shoreFacade;
  std::unique_ptr<BuildableFacade> buildableFacade;
  std::shared_ptr<PlantGeneratorFacade> plantGeneratorFacade;
  Skybox skybox;
  TheSun theSun;
  std::unique_ptr<UnderwaterFacade> underwaterFacade;
};

#endif // TERRAINGENERATORFACADE_H
