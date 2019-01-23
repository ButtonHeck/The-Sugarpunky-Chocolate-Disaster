#include "game/world/models/plants/TreesRenderer.h"

void TreesRenderer::render(std::vector<Model> &landModels, std::vector<Model> &hillModels, bool isShadow)
{
  BenchmarkTimer BENCHMARK_;
  if (!isShadow)
    BENCHMARK_("TreesRenderer: render", true);
  else
    BENCHMARK_("TreesRenderer: render depthmap", true);

  for (unsigned int i = 0; i < landModels.size(); i++)
    {
      Model& model = landModels[i];
      model.draw(isShadow);
    }

  for (unsigned int i = 0; i < hillModels.size(); i++)
    {
      Model& model = hillModels[i];
      model.draw(isShadow);
    }
}
