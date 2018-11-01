#include "TheSunRenderer.h"

TheSunRenderer::TheSunRenderer(TheSun &theSun)
  :
    theSun(theSun)
{}

void TheSunRenderer::render()
{
  BENCHMARK("SunRenderer: draw", true);
  glPointSize(64.0f);
  RendererStateManager::setAmbienceRenderingState(true);
  theSun.basicGLBuffers.bind(VAO);
  glDrawArrays(GL_POINTS, 0, 1);
  RendererStateManager::setAmbienceRenderingState(false);
  glPointSize(1.0f);
}
