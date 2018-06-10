#ifndef RENDERER_H
#define RENDERER_H
#include <glm/glm.hpp>
#include "Shader.h"
#include "Camera.h"
#include "HillsMapGenerator.h"
#include "BaseMapGenerator.h"
#include "UnderwaterQuadMapGenerator.h"
#include "BuildableMapGenerator.h"
#include "WaterMapGenerator.h"
#include "Skybox.h"
#include "TreeGenerator.h"

class Renderer
{
public:
  Renderer(Camera& camera);
  void updateDrawVariables();
  void drawHills(HillsMapGenerator* generator, bool enableFrustumCulling);
  void drawShore(BaseMapGenerator* generator);
  void drawFlatTerrain(BaseMapGenerator* generator);
  void drawUnderwaterQuad(UnderwaterQuadMapGenerator* generator);
  void drawBuildableTiles(BuildableMapGenerator* generator);
  void drawSelectedTile(BuildableMapGenerator* generator);
  void drawWater(WaterMapGenerator* generator, bool animateWater);
  void drawSkybox(Skybox* skybox);
  void drawTrees(TreeGenerator* generator, Shader& shader, bool enableFrustumCulling);
private:
  Camera& camera;
  int cameraOnMapCoordX, cameraOnMapCoordZ;
  glm::vec2 cameraPosition;
  glm::vec2 viewDirection;
  float cameraCorrectedFOVDOT;
};

#endif // RENDERER_H
