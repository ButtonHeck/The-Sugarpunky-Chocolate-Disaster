#ifndef RENDERER_H
#define RENDERER_H
#include <glm/vec2.hpp>
#include <glm/detail/func_exponential.hpp>
#include "graphics/Shader.h"
#include "graphics/Camera.h"
#include "graphics/Frustum.h"
#include "graphics/Skybox.h"
#include "generator/HillsMapGenerator.h"
#include "generator/BaseMapGenerator.h"
#include "generator/UnderwaterQuadMapGenerator.h"
#include "generator/BuildableMapGenerator.h"
#include "generator/WaterMapGenerator.h"
#include "generator/TreeGenerator.h"

class Renderer
{
public:
  Renderer(Camera& camera);
  void drawHills(bool useFC, HillsMapGenerator* generator, Shader &fc, Shader &nofc);
  void drawShore(BaseMapGenerator* generator);
  void drawFlatTerrain(BaseMapGenerator* generator, Frustum& frustum);
  void drawUnderwaterQuad(UnderwaterQuadMapGenerator* generator);
  void drawBuildableTiles(BuildableMapGenerator* generator);
  void drawSelectedTile(BuildableMapGenerator* generator);
  void drawWater(bool useFC, WaterMapGenerator* generator, Shader& fc, Shader& nofc);
  void drawSkybox(Skybox* skybox);
  void drawTrees(TreeGenerator* generator, Shader& shader, bool enableFrustumCulling, Frustum &frustum);
private:
  Camera& camera;
};

#endif // RENDERER_H
