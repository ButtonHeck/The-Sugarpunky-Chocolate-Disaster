#ifndef PLANTSRENDERER_H
#define PLANTSRENDERER_H
#include <memory>
#include <graphics/Shader.h>
#include <model/Model.h>

class PlantsRenderer
{
public:
  PlantsRenderer() = default;
  void render(std::vector<Model>& landModels,
              std::vector<Model>& hillModels,
              std::vector<Model>& grassModels,
              Shader& shader,
              bool bindTexture,
              bool screenDraw,
              bool useFlatBlending);
};

#endif // PLANTSRENDERER_H
