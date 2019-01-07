#include "LensFlareRenderer.h"

LensFlareRenderer::LensFlareRenderer(OpenglBuffer &basicGLBuffer)
  :
    basicGLBuffer(basicGLBuffer)
{}

void LensFlareRenderer::draw(unsigned int numFlares)
{
  basicGLBuffer.bind(VAO);
  glEnable(GL_BLEND);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glDepthMask(GL_FALSE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glDepthFunc(GL_LEQUAL);
  glDrawArrays(GL_POINTS, 0, numFlares);
  glDepthFunc(GL_LESS);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(GL_TRUE);
  glDisable(GL_PROGRAM_POINT_SIZE);
  glDisable(GL_BLEND);
}
