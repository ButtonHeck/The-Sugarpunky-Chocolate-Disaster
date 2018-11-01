#include "GrassRenderer.h"

void GrassRenderer::render(std::vector<Model> &models,
                           bool bindTexture,
                           bool useFlatBlending)
{
  BenchmarkTimer BENCHMARK_;
  if (bindTexture)
    BENCHMARK_("GrassRenderer: render", true);
  else
    BENCHMARK_("GrassRenderer: render depthmap", true);

  if (useFlatBlending)
    glEnable(GL_BLEND);
  else
    glDisable(GL_BLEND);

  glDisable(GL_CULL_FACE);
  for (unsigned int i = 0; i < models.size(); i++)
    {
      Model& model = models[i];
      model.draw(bindTexture);
    }
  glEnable(GL_CULL_FACE);

  if (useFlatBlending)
    glDisable(GL_BLEND);
}
