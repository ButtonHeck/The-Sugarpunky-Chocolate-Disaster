#include "ShoreRenderer.h"

ShoreRenderer::ShoreRenderer(ShoreGenerator &generator)
  :
    generator(generator)
{}

void ShoreRenderer::render()
{
  generator.basicGLBuffers.bind(VAO);
  glEnable(GL_BLEND);
  glDrawArrays(GL_TRIANGLES, 0, generator.tiles.size() * VERTICES_PER_TILE);
  glDisable(GL_BLEND);
}

void ShoreRenderer::debugRender(GLenum primitiveType)
{
  generator.basicGLBuffers.bind(VAO);
  glLineWidth(2.0f);
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDrawArrays(primitiveType, 0, generator.tiles.size() * VERTICES_PER_TILE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);
  glLineWidth(1.0f);
}
