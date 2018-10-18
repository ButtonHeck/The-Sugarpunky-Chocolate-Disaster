#ifndef RENDERER_H
#define RENDERER_H
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "graphics/Shader.h"
#include "game/Camera.h"
#include "graphics/Frustum.h"
#include "game/world/Skybox.h"
#include "game/world/terrain/HillsGenerator.h"
#include "game/world/terrain/LandGenerator.h"
#include "game/world/terrain/ShoreGenerator.h"
#include "game/world/terrain/UnderwaterGenerator.h"
#include "game/world/terrain/BuildableGenerator.h"
#include "game/world/terrain/WaterGenerator.h"
#include "game/world/PlantGeneratorFacade.h"

class Renderer
{
public:
  Renderer(Camera& camera);
  static void setInitialGLState(bool useMultisample);
  void renderHills(bool useFC, const std::shared_ptr<HillsGenerator> generator, Shader &fc, Shader &nofc);
  void renderHillsDepthmap(const std::shared_ptr<HillsGenerator> generator);
  void renderShore(const std::shared_ptr<ShoreGenerator> generator);
  void renderLand(const std::shared_ptr<LandGenerator> generator, Frustum& frustum, GLuint texture);
  void renderUnderwater(UnderwaterGenerator &generator);
  void renderBuildableTiles(const std::shared_ptr<BuildableGenerator> generator);
  void renderSelectedTile(const std::shared_ptr<BuildableGenerator> generator);
  void renderWater(bool useFC, std::shared_ptr<WaterGenerator> generator, Shader& fc, Shader& nofc);
  void renderSkybox(Skybox* skybox);
  void renderPlants(const std::shared_ptr<PlantGeneratorFacade> generatorFacade, Shader& shader,
                 bool bindTexture,
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
