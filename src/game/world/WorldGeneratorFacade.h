#ifndef TERRAINGENERATORFACADE_H
#define TERRAINGENERATORFACADE_H
#include <memory>
#include "game/world/terrain/LandGenerator.h"
#include "game/world/terrain/hills/HillsFacade.h"
#include "game/world/PlantGeneratorFacade.h"
#include "game/world/terrain/WaterGenerator.h"
#include "game/world/terrain/BuildableGenerator.h"
#include "game/world/terrain/ShoreGenerator.h"
#include "game/world/terrain/UnderwaterGenerator.h"
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
  const std::shared_ptr<WaterGenerator> getWaterGenerator() const;
  const std::shared_ptr<HillsFacade> getHillsFacade() const;
  const std::shared_ptr<PlantGeneratorFacade> getPlantsGeneratorFacade() const;

private:
  //rendering on screen
  void drawFlatTerrain(Frustum &viewFrustum);
  void drawUnderwater();
  void drawShore();
  void drawPlants(glm::vec3& viewPosition);
  void drawBuildable();
  void drawSelected(MouseInputManager& mouseInput, Camera& camera);
  void drawWater(glm::vec3 &viewPosition, Frustum &viewFrustum);
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
  std::shared_ptr<WaterGenerator> waterGenerator;
  std::shared_ptr<HillsFacade> hillsFacade;
  std::shared_ptr<LandGenerator> landGenerator;
  std::shared_ptr<ShoreGenerator> shoreGenerator;
  std::shared_ptr<BuildableGenerator> buildableGenerator;
  std::shared_ptr<PlantGeneratorFacade> plantGeneratorFacade;
  Skybox skybox;
  TheSun theSun;
  UnderwaterGenerator underwaterGenerator;
};

#endif // TERRAINGENERATORFACADE_H
