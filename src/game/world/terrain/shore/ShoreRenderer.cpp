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
