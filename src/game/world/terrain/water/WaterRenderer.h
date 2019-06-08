#ifndef WATERRENDERER_H
#define WATERRENDERER_H
#include "WaterGenerator"
#include "WaterShader"
#include "BenchmarkTimer"
#include "Query"

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
