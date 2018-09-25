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
#include "generator/PlantGenerator.h"

class Renderer
{
public:
  Renderer(Camera& camera);
  void drawHills(bool useFC, HillsMapGenerator* generator, Shader &fc, Shader &nofc);
  void drawHillsDepthmap(HillsMapGenerator* generator);
  void drawShore(BaseMapGenerator* generator);
  void drawFlatTerrain(BaseMapGenerator* generator, Frustum& frustum);
  void addIndirectBufferData(GLuint& primCount,
                             GLuint* buffer,
                             GLuint& dataOffset,
                             GLuint numInstances,
                             GLuint instanceOffset);
  void drawUnderwaterQuad(UnderwaterQuadMapGenerator* generator);
  void drawBuildableTiles(BuildableMapGenerator* generator);
  void drawSelectedTile(BuildableMapGenerator* generator);
  void drawWater(bool useFC, WaterMapGenerator* generator, Shader& fc, Shader& nofc);
  void drawSkybox(Skybox* skybox);
  void drawPlants(PlantGenerator* generator, Shader& shader,
                 bool enableFrustumCulling,
                 bool bindTexture,
                 bool updateIndirect,
                 bool screenDraw,
                 bool useFlatBlending);
private:
  Camera& camera;
};

#endif // RENDERER_H
