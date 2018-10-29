#ifndef WATERRENDERER_H
#define WATERRENDERER_H
#include <memory>
#include "game/world/terrain/water/WaterGenerator.h"
#include "game/world/terrain/water/WaterShader.h"
#include "util/BenchmarkTimer.h"

class WaterRenderer
{
public:
  WaterRenderer(WaterShader& shaders, std::shared_ptr<WaterGenerator> generator);
  void render(bool useCulling);
private:
  WaterShader& shaders;
  std::shared_ptr<WaterGenerator> generator;
};

#endif // WATERRENDERER_H
