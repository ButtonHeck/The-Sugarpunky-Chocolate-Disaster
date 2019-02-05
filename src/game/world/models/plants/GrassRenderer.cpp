#include "game/world/models/plants/GrassRenderer.h"

void GrassRenderer::render(std::vector<Model> &models, bool isShadow)
{
  BenchmarkTimer BENCHMARK_;
  if (!isShadow)
    BENCHMARK_("GrassRenderer: render", true);
  else
    BENCHMARK_("GrassRenderer: render depthmap", true);

  glDisable(GL_CULL_FACE);
  for (unsigned int modelIndex = 0; modelIndex < models.size(); modelIndex++)
    {
      Model& model = models[modelIndex];
      model.draw(isShadow);
    }
  glEnable(GL_CULL_FACE);
}
