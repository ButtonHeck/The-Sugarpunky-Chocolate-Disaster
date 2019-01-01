#include "TheSunRenderer.h"

TheSunRenderer::TheSunRenderer(TheSun &theSun)
  :
    theSun(theSun)
{
  glCreateQueries(GL_SAMPLES_PASSED, 1, &samplesPassedQuery);
}

TheSunRenderer::~TheSunRenderer()
{
  glDeleteQueries(1, &samplesPassedQuery);
}

void TheSunRenderer::render()
{
  BENCHMARK("SunRenderer: draw", true);
  glPointSize(SUN_POINT_SIZE);
  RendererStateManager::setAmbienceRenderingState(true);
  theSun.basicGLBuffers.bind(VAO);
  glBeginQuery(GL_SAMPLES_PASSED, samplesPassedQuery);
  glDrawArrays(GL_POINTS, 0, 1);
  glEndQuery(GL_SAMPLES_PASSED);
  RendererStateManager::setAmbienceRenderingState(false);
  glPointSize(1.0f);
}

GLuint TheSunRenderer::getSamplesPassedQueryResult()
{
  glGetQueryObjectuiv(samplesPassedQuery, GL_QUERY_RESULT, &samplesPassedResult);
  return samplesPassedResult;
}
