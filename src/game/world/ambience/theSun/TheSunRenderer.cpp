#include "TheSunRenderer.h"

TheSunRenderer::TheSunRenderer(TheSun &theSun)
  :
    theSun(theSun),
    samplesPassedQuery(GL_SAMPLES_PASSED)
{}

void TheSunRenderer::render(bool doOcclusionTest)
{
  BENCHMARK("SunRenderer: draw", true);
  glPointSize(SUN_POINT_SIZE);
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
    samplesPassedResult = samplesPassedQuery.getResult();
  glPointSize(1.0f);
}

GLuint TheSunRenderer::getSamplesPassedQueryResult()
{
  return samplesPassedResult;
}
