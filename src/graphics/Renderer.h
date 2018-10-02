#ifndef RENDERER_H
#define RENDERER_H
#include <glm/vec2.hpp>
#include <glm/detail/func_exponential.hpp>
#include <memory>
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
  void drawHills(bool useFC, const std::shared_ptr<HillsMapGenerator> generator, Shader &fc, Shader &nofc);
  void drawHillsDepthmap(const std::shared_ptr<HillsMapGenerator> generator);
  void drawShore(const std::shared_ptr<BaseMapGenerator> generator);
  void drawFlatTerrain(const std::shared_ptr<BaseMapGenerator> generator, Frustum& frustum, GLuint texture);
  void addIndirectBufferData(GLuint& primCount,
                             GLuint* buffer,
                             GLuint& dataOffset,
                             GLuint numInstances,
                             GLuint instanceOffset);
  void drawUnderwaterQuad(const UnderwaterQuadMapGenerator& generator);
  void drawBuildableTiles(const std::shared_ptr<BuildableMapGenerator> generator);
  void drawSelectedTile(const std::shared_ptr<BuildableMapGenerator> generator);
  void drawWater(bool useFC, std::shared_ptr<WaterMapGenerator> generator, Shader& fc, Shader& nofc);
  void drawSkybox(Skybox* skybox);
  void drawPlants(const std::shared_ptr<PlantGenerator> generator, Shader& shader,
                 bool enableFrustumCulling,
                 bool bindTexture,
                 bool updateIndirect,
                 bool screenDraw,
                 bool useFlatBlending);
private:
  Camera& camera;
};

#endif // RENDERER_H
