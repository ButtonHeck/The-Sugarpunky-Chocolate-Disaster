#ifndef RENDERER_H
#define RENDERER_H
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "game/world/Skybox.h"
#include "game/world/TheSun.h"
#include "game/world/PlantGeneratorFacade.h"

class Shader;
class Frustum;

class Renderer
{
public:
  Renderer() = default;
  static void setInitialGLState(bool useMultisample);
  void renderSkybox(Skybox* skybox);
  void renderSun(TheSun* theSun);
  void renderPlants(const std::shared_ptr<PlantGeneratorFacade> generatorFacade, Shader& shader,
                 bool bindTexture,
                 bool screenDraw,
                 bool useFlatBlending);
private:
  static void setAmbientRenderingState(bool isOn);
};

#endif // RENDERER_H
