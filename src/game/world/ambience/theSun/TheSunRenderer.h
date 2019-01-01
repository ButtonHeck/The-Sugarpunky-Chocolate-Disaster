#ifndef THESUNRENDERER_H
#define THESUNRENDERER_H
#include "game/world/ambience/theSun/TheSun.h"
#include "graphics/RendererStateManager.h"
#include "util/BenchmarkTimer.h"

constexpr float SUN_POINT_SIZE = 64.0f;

class TheSunRenderer
{
public:
  TheSunRenderer(TheSun& theSun);
  virtual ~TheSunRenderer();
  void render();
  GLuint getSamplesPassedQueryResult();
private:
  TheSun& theSun;
  GLuint samplesPassedQuery;
  GLuint samplesPassedResult;
};

#endif // THESUNRENDERER_H
