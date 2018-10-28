#ifndef RENDERER_H
#define RENDERER_H
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "game/world/Skybox.h"
#include "game/world/TheSun.h"
#include "game/world/terrain/LandGenerator.h"
#include "game/world/terrain/WaterGenerator.h"
#include "game/world/PlantGeneratorFacade.h"

class Camera;
class Shader;
class Frustum;

class Renderer
{
public:
  Renderer(Camera& camera);
  static void setInitialGLState(bool useMultisample);
  void renderLand(const std::shared_ptr<LandGenerator> generator, Frustum& frustum, GLuint texture);
  void renderWater(bool useFC, std::shared_ptr<WaterGenerator> generator, Shader& fc, Shader& nofc);
  void renderSkybox(Skybox* skybox);
  void renderSun(TheSun* theSun);
  void renderPlants(const std::shared_ptr<PlantGeneratorFacade> generatorFacade, Shader& shader,
                 bool bindTexture,
                 bool screenDraw,
                 bool useFlatBlending);
private:
  static void setAmbientRenderingState(bool isOn);
  void addIndirectBufferData(GLuint& primCount,
                             GLuint* buffer,
                             GLuint& dataOffset,
                             GLuint numInstances,
                             GLuint instanceOffset);
  Camera& camera;
};

#endif // RENDERER_H
