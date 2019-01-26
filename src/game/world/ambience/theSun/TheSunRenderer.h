#ifndef THESUNRENDERER_H
#define THESUNRENDERER_H
#include "game/world/ambience/theSun/TheSun.h"
#include "graphics/RendererStateManager.h"
#include "util/BenchmarkTimer.h"
#include "graphics/openglObjects/Query.h"

class TheSunRenderer
{
public:
  TheSunRenderer(TheSun& theSun);
  virtual ~TheSunRenderer() = default;
  void render(bool doOcclusionTest, bool useReflectionPointSize);
  GLuint getSamplesPassedQueryResult();
  void setPointSize(float pointSize);
  void setReflectionPointSize(float pointSize);
  float getPointSize() const;
private:
  TheSun& theSun;
  Query samplesPassedQuery;
  GLuint samplesPassedResult;
  float pointSize;
  float reflectionPointSize;
};

#endif // THESUNRENDERER_H
