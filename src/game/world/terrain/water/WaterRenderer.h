#ifndef WATERRENDERER_H
#define WATERRENDERER_H
#include "game/world/terrain/water/WaterGenerator.h"
#include "game/world/terrain/water/WaterShader.h"
#include "util/BenchmarkTimer.h"
#include "graphics/openglObjects/Query.h"

class WaterRenderer
{
public:
  WaterRenderer(WaterShader& shaders, WaterGenerator& generator);
  void render(bool useFrustumCulling);
  void debugRender(GLenum primitiveType);
  bool anySamplesPassed() const;
private:
  WaterShader& shaders;
  WaterGenerator& generator;
  Query anySamplesPassedQuery;
};

#endif // WATERRENDERER_H
