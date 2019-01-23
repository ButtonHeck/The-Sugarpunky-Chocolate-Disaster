#ifndef GRASSRENDERER_H
#define GRASSRENDERER_H
#include "graphics/shaders/Shader.h"
#include "game/world/models/Model.h"

class GrassRenderer
{
public:
  GrassRenderer() = default;
  void render(std::vector<Model>& models, bool isShadow);
};

#endif // GRASSRENDERER_H
