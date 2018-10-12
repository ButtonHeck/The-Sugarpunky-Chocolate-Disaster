#ifndef TERRAINGENERATORFACADE_H
#define TERRAINGENERATORFACADE_H
#include <memory>
#include "generator/BaseMapGenerator.h"
#include "generator/HillsMapGenerator.h"
#include "generator/PlantGeneratorFacade.h"
#include "generator/WaterMapGenerator.h"
#include "generator/BuildableMapGenerator.h"
#include "generator/ShoreGenerator.h"
#include "generator/UnderwaterQuadMapGenerator.h"
#include "graphics/ShaderManager.h"
#include "graphics/Renderer.h"
#include "graphics/TextureManager.h"
#include "input/MouseInputManager.h"

class WorldGeneratorFacade
{
public:
  WorldGeneratorFacade(ShaderManager& shaderManager, Renderer& renderer, Options& options);

  //internal generators functions
  void prepareTerrain();
  void recreate();
  void load();
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);
  void bufferWaterNewData();

  //rendering part
  void drawWorld(glm::mat4& projectionView,
                 glm::vec3 &viewPosition,
                 Frustum &viewFrustum,
                 TextureManager& textureManager,
                 Camera& camera,
                 MouseInputManager& mouseInput,
                 unsigned long updateCount);
  void drawWorldDepthmap(unsigned long updateCount);

  //getters
  const std::shared_ptr<WaterMapGenerator> getWaterGenerator() const;
  const std::shared_ptr<HillsMapGenerator> getHillsGenerator() const;
  const std::shared_ptr<PlantGeneratorFacade> getPlantsGeneratorFacade() const;

private:
  //rendering on screen
  void drawHills(glm::vec3& viewPosition, Frustum& viewFrustum);
  void drawFlatTerrain(Frustum &viewFrustum, TextureManager& textureManager);
  void drawUnderwater();
  void drawShore();
  void drawPlants(glm::vec3& viewPosition, unsigned long updateCount);
  void drawBuildable();
  void drawSelected(MouseInputManager& mouseInput, Camera& camera);
  void drawWater(glm::vec3 &viewPosition, Frustum &viewFrustum);

  //rendering offscreen (depthmap)
  void drawTerrainDepthmap();
  void drawPlantsDepthmap(unsigned long updateCount);

  //internals
  ShaderManager& shaderManager;
  Renderer& renderer;
  Options& options;
  glm::mat4 projectionView;
  std::shared_ptr<WaterMapGenerator> waterMapGenerator;
  std::shared_ptr<HillsMapGenerator> hillMapGenerator;
  std::shared_ptr<BaseMapGenerator> baseMapGenerator;
  std::shared_ptr<ShoreGenerator> shoreGenerator;
  std::shared_ptr<BuildableMapGenerator> buildableMapGenerator;
  std::shared_ptr<PlantGeneratorFacade> plantGeneratorFacade;
  UnderwaterQuadMapGenerator underwaterQuadGenerator;
};

#endif // TERRAINGENERATORFACADE_H
