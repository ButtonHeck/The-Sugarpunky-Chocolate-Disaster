#include "TheSunRenderer.h"

TheSunRenderer::TheSunRenderer(TheSun &theSun)
  :
    theSun(theSun)
{}

void TheSunRenderer::render()
{
  glPointSize(64.0f);
  Renderer::setAmbientRenderingState(true);
  theSun.basicGLBuffers.bind(VAO);
  glDrawArrays(GL_POINTS, 0, 1);
  Renderer::setAmbientRenderingState(false);
  glPointSize(1.0f);
}
