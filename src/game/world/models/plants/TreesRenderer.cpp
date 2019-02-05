#include "game/world/models/plants/TreesRenderer.h"

void TreesRenderer::render(std::vector<Model> &landModels, std::vector<Model> &hillModels, bool isShadow)
{
  BenchmarkTimer BENCHMARK_;
  if (!isShadow)
    BENCHMARK_("TreesRenderer: render", true);
  else
    BENCHMARK_("TreesRenderer: render depthmap", true);

  for (unsigned int modelIndex = 0; modelIndex < landModels.size(); modelIndex++)
    {
      Model& model = landModels[modelIndex];
      model.draw(isShadow);
    }

  for (unsigned int modelIndex = 0; modelIndex < hillModels.size(); modelIndex++)
    {
      Model& model = hillModels[modelIndex];
      model.draw(isShadow);
    }
}
