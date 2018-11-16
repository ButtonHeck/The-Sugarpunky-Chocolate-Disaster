#ifndef GRASSRENDERER_H
#define GRASSRENDERER_H
#include <graphics/Shader.h>
#include <model/Model.h>

class GrassRenderer
{
public:
  GrassRenderer() = default;
  void render(std::vector<Model>& models,
              bool bindTexture,
              bool useLandBlending);
};

#endif // GRASSRENDERER_H
