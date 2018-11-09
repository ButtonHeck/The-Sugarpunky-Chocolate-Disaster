#ifndef WATERRENDERER_H
#define WATERRENDERER_H
#include "game/world/terrain/water/WaterGenerator.h"
#include "game/world/terrain/water/WaterShader.h"
#include "util/BenchmarkTimer.h"

class WaterRenderer
{
public:
  WaterRenderer(WaterShader& shaders, WaterGenerator& generator);
  void render(bool useCulling);
  void debugRender(GLenum primitiveType);
private:
  WaterShader& shaders;
  WaterGenerator& generator;
};

#endif // WATERRENDERER_H
