#include "GrassRenderer.h"

void GrassRenderer::render(std::vector<Model> &models,
                           bool useLandBlending,
                           bool isShadow)
{
  BenchmarkTimer BENCHMARK_;
  if (!isShadow)
    BENCHMARK_("GrassRenderer: render", true);
  else
    BENCHMARK_("GrassRenderer: render depthmap", true);

  if (useLandBlending)
    glEnable(GL_BLEND);
  else
    glDisable(GL_BLEND);

  glDisable(GL_CULL_FACE);
  for (unsigned int i = 0; i < models.size(); i++)
    {
      Model& model = models[i];
      model.draw(isShadow);
    }
  glEnable(GL_CULL_FACE);

  if (useLandBlending)
    glDisable(GL_BLEND);
}
