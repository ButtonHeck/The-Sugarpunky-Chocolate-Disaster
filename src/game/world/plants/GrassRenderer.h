#ifndef GRASSRENDERER_H
#define GRASSRENDERER_H
#include <graphics/Shader.h>
#include <model/Model.h>

class GrassRenderer
{
public:
  GrassRenderer() = default;
  void render(std::vector<Model>& models, bool isShadow);
};

#endif // GRASSRENDERER_H
