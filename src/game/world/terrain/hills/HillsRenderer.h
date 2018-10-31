#ifndef HILLSRENDERER_H
#define HILLSRENDERER_H
#include "game/world/terrain/hills/HillsGenerator.h"
#include "game/world/terrain/hills/HillsShader.h"
#include "util/BenchmarkTimer.h"

class HillsRenderer
{
public:
  HillsRenderer(HillsShader& shaders, HillsGenerator& generator);
  void render(bool useFC);
  void renderDepthmap();
private:
  HillsShader& shaders;
  HillsGenerator& generator;
};

#endif // HILLSRENDERER_H
