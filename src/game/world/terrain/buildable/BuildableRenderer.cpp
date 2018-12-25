#include "BuildableRenderer.h"

BuildableRenderer::BuildableRenderer(BuildableGenerator &generator)
  :
    generator(generator)
{}

void BuildableRenderer::renderBuildable()
{
  generator.basicGLBuffers.bind(VAO);
  glEnable(GL_BLEND);
  glDrawElementsInstanced(GL_TRIANGLES, VERTICES_PER_QUAD, GL_UNSIGNED_BYTE, 0, generator.tiles.size());
  glDisable(GL_BLEND);
}

void BuildableRenderer::renderSelected()
{
  generator.selectedBuffers.bind(VAO);
  glEnable(GL_BLEND);
  glDrawElements(GL_TRIANGLES, VERTICES_PER_QUAD, GL_UNSIGNED_BYTE, 0);
  glDisable(GL_BLEND);
}
