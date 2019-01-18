#ifndef THESUNRENDERER_H
#define THESUNRENDERER_H
#include "game/world/ambience/theSun/TheSun.h"
#include "graphics/RendererStateManager.h"
#include "util/BenchmarkTimer.h"
#include "graphics/Query.h"

class TheSunRenderer
{
public:
  TheSunRenderer(TheSun& theSun);
  virtual ~TheSunRenderer() = default;
  void render(bool doOcclusionTest);
  GLuint getSamplesPassedQueryResult();
  void setPointSize(float pointSize);
  float getPointSize() const;
private:
  TheSun& theSun;
  Query samplesPassedQuery;
  GLuint samplesPassedResult;
  float pointSize;
};

#endif // THESUNRENDERER_H
