#ifndef WATERRENDERER_H
#define WATERRENDERER_H
#include "WaterGenerator.h"
#include "WaterShader.h"
#include "../../src/util/BenchmarkTimer.h"
#include "../../src/graphics/openglObjects/Query.h"

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
