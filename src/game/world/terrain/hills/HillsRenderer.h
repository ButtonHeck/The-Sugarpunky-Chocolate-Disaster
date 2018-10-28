#ifndef HILLSRENDERER_H
#define HILLSRENDERER_H
#include <memory>
#include "game/world/terrain/hills/HillsGenerator.h"
#include "game/world/terrain/hills/HillsShader.h"
#include "util/BenchmarkTimer.h"

class HillsRenderer
{
public:
  HillsRenderer(HillsShader& shaders, std::shared_ptr<HillsGenerator> generator);
  void render(bool useFC);
  void renderDepthmap();
private:
  HillsShader& shaders;
  std::shared_ptr<HillsGenerator> generator;
};

#endif // HILLSRENDERER_H
