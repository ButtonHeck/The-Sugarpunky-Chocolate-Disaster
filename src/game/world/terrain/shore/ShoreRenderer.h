#pragma once

#include <GL/glew.h>

class ShoreGenerator;

class ShoreRenderer
{
public:
  ShoreRenderer(ShoreGenerator& generator) noexcept;
  void render();
  void debugRender(GLenum primitiveType);

private:
  ShoreGenerator& generator;
};
