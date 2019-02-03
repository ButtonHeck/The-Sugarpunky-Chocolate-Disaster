#ifndef THESUNRENDERER_H
#define THESUNRENDERER_H
#include "game/world/ambience/theSun/TheSun.h"
#include "graphics/openglObjects/Query.h"
#include "graphics/RendererStateManager.h"
#include "util/BenchmarkTimer.h"

class TheSunRenderer
{
public:
  TheSunRenderer(TheSun& theSun);
  virtual ~TheSunRenderer() = default;
  void render(bool doOcclusionTest, bool useReflectionPointSize);
  GLuint getSamplesPassedQueryResult() const;
  void setPointSize(float pointSize);
  void setReflectionPointSize(float pointSize);
  float getPointSize() const;

private:
  TheSun& theSun;
  Query samplesPassedQuery;
  float pointSize;
  float reflectionPointSize;
};

#endif // THESUNRENDERER_H
