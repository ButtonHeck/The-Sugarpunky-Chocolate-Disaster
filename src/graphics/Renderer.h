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
#include "generator/ShoreGenerator.h"
#include "generator/UnderwaterQuadMapGenerator.h"
#include "generator/BuildableMapGenerator.h"
#include "generator/WaterMapGenerator.h"
#include "generator/PlantGeneratorFacade.h"

class Renderer
{
public:
  Renderer(Camera& camera);
  void renderHills(bool useFC, const std::shared_ptr<HillsMapGenerator> generator, Shader &fc, Shader &nofc);
  void renderHillsDepthmap(const std::shared_ptr<HillsMapGenerator> generator);
  void renderShore(const std::shared_ptr<ShoreGenerator> generator);
  void renderFlatTerrain(const std::shared_ptr<BaseMapGenerator> generator, Frustum& frustum, GLuint texture);
  void renderUnderwater(const UnderwaterQuadMapGenerator& generator);
  void renderBuildableTiles(const std::shared_ptr<BuildableMapGenerator> generator);
  void renderSelectedTile(const std::shared_ptr<BuildableMapGenerator> generator);
  void renderWater(bool useFC, std::shared_ptr<WaterMapGenerator> generator, Shader& fc, Shader& nofc);
  void renderSkybox(Skybox* skybox);
  void renderPlants(const std::shared_ptr<PlantGeneratorFacade> generatorFacade, Shader& shader,
                 bool enableFrustumCulling,
                 bool bindTexture,
                 bool updateIndirect,
                 bool screenDraw,
                 bool useFlatBlending);
private:
  void addIndirectBufferData(GLuint& primCount,
                             GLuint* buffer,
                             GLuint& dataOffset,
                             GLuint numInstances,
                             GLuint instanceOffset);
  Camera& camera;
};

#endif // RENDERER_H
