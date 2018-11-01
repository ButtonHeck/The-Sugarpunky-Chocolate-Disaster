#include "TreesRenderer.h"

void TreesRenderer::render(std::vector<Model> &landModels,
                           std::vector<Model> &hillModels,
                           bool bindTexture,
                           bool useFlatBlending)
{
  BenchmarkTimer BENCHMARK_;
  if (bindTexture)
    BENCHMARK_("TreesRenderer: render", true);
  else
    BENCHMARK_("TreesRenderer: render depthmap", true);

  if (useFlatBlending)
    glEnable(GL_BLEND);
  else
    glDisable(GL_BLEND);

  for (unsigned int i = 0; i < landModels.size(); i++)
    {
      Model& model = landModels[i];
      model.draw(bindTexture);
    }

  for (unsigned int i = 0; i < hillModels.size(); i++)
    {
      Model& model = hillModels[i];
      model.draw(bindTexture);
    }

  if (useFlatBlending)
    glDisable(GL_BLEND);
}
