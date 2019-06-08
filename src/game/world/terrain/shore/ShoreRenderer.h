#pragma once

#include "ShoreGenerator"

class ShoreRenderer
{
public:
  ShoreRenderer(ShoreGenerator& generator);
  void render();
  void debugRender(GLenum primitiveType);

private:
  ShoreGenerator& generator;
};
