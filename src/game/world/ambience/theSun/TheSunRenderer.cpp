#include "game/world/ambience/theSun/TheSunRenderer.h"

TheSunRenderer::TheSunRenderer(TheSun& theSun)
  :
    theSun(theSun),
    samplesPassedQuery(GL_SAMPLES_PASSED),
    pointSize(0.0f)
{}

void TheSunRenderer::render(bool doOcclusionTest, bool useReflectionPointSize)
{
  BENCHMARK("SunRenderer: draw", true);
  glPointSize(useReflectionPointSize ? reflectionPointSize : pointSize);
  theSun.basicGLBuffers.bind(VAO);
  if (doOcclusionTest && !samplesPassedQuery.isInUse())
    {
      samplesPassedQuery.start();
      glDrawArrays(GL_POINTS, 0, 1);
      samplesPassedQuery.end();
    }
  else
    glDrawArrays(GL_POINTS, 0, 1);

  if (samplesPassedQuery.isResultAvailable())
    samplesPassedQuery.requestResult();
  glPointSize(1.0f);
}

GLuint TheSunRenderer::getSamplesPassedQueryResult() const
{
  return samplesPassedQuery.getResult();
}

void TheSunRenderer::setPointSize(float pointSize)
{
  this->pointSize = pointSize;
}

void TheSunRenderer::setReflectionPointSize(float pointSize)
{
  this->reflectionPointSize = pointSize;
}

float TheSunRenderer::getPointSize() const
{
  return pointSize;
}
