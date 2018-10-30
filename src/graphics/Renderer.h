#ifndef RENDERER_H
#define RENDERER_H
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "game/world/PlantGeneratorFacade.h"

class Shader;

class Renderer
{
public:
  Renderer() = default;
  static void setInitialGLState(bool useMultisample);
  static void setAmbientRenderingState(bool isOn);
  void renderPlants(const std::shared_ptr<PlantGeneratorFacade> generatorFacade, Shader& shader,
                 bool bindTexture,
                 bool screenDraw,
                 bool useFlatBlending);
private:
};

#endif // RENDERER_H
