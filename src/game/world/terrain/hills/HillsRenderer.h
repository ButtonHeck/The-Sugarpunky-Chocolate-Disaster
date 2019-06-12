#pragma once

#include <GL/glew.h>
#include <glm/vec2.hpp>

class HillsGenerator;
class HillsShader;

class HillsRenderer
{
public:
  HillsRenderer(HillsShader& shaders, HillsGenerator& generator) noexcept;
  void render(bool useFrustumCulling, const glm::vec2 &viewAcceleration);
  void renderDepthmap();
  void debugRender(GLenum primitiveType);

private:
  const unsigned int FEEDBACK_UPDATE_DELAY = 12;
  HillsShader& shaders;
  HillsGenerator& generator;
};
