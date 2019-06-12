#pragma once

#include "Query"

class WaterGenerator;
class WaterShader;

class WaterRenderer
{
public:
  WaterRenderer(WaterShader& shaders, WaterGenerator& generator) noexcept;
  void render(bool useFrustumCulling);
  void debugRender(GLenum primitiveType);
  bool anySamplesPassed() const noexcept;

private:
  WaterShader& shaders;
  WaterGenerator& generator;
  Query anySamplesPassedQuery;
};
