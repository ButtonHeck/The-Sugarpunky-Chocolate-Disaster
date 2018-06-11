#ifndef RENDERER_H
#define RENDERER_H
#include <glm/glm.hpp>
#include "src/graphics/Shader.h"
#include "src/graphics/Camera.h"
#include "src/graphics/Frustum.h"
#include "src/generator/HillsMapGenerator.h"
#include "src/generator/BaseMapGenerator.h"
#include "src/generator/UnderwaterQuadMapGenerator.h"
#include "src/generator/BuildableMapGenerator.h"
#include "src/generator/WaterMapGenerator.h"
#include "src/graphics/Skybox.h"
#include "src/generator/TreeGenerator.h"

class Renderer
{
public:
  Renderer(Camera& camera);
  void updateDrawVariables();
  void drawHills(HillsMapGenerator* generator);
  void drawShore(BaseMapGenerator* generator, Frustum& frustum);
  void drawFlatTerrain(BaseMapGenerator* generator);
  void drawUnderwaterQuad(UnderwaterQuadMapGenerator* generator);
  void drawBuildableTiles(BuildableMapGenerator* generator);
  void drawSelectedTile(BuildableMapGenerator* generator);
  void drawWater(WaterMapGenerator* generator, bool animateWater);
  void drawSkybox(Skybox* skybox);
  void drawTrees(TreeGenerator* generator, Shader& shader, bool enableFrustumCulling,
                 std::vector<ModelChunk>& treeModelChunks, std::vector<ModelChunk>& hillTreeModelChunks);
private:
  Camera& camera;
  int cameraOnMapCoordX, cameraOnMapCoordZ;
  glm::vec2 cameraPosition;
  glm::vec2 viewDirection;
  float cameraCorrectedFOVDOT;
};

#endif // RENDERER_H
