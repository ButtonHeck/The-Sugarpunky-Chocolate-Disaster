#ifndef TREESRENDERER_H
#define TREESRENDERER_H
#include <graphics/Shader.h>
#include <model/Model.h>

class TreesRenderer
{
public:
  TreesRenderer() = default;
  void render(std::vector<Model>& landModels,
              std::vector<Model>& hillModels,
              bool bindTexture,
              bool useFlatBlending);
};

#endif // TREESRENDERER_H