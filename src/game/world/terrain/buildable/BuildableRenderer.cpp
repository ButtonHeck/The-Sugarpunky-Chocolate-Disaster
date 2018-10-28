#include "BuildableRenderer.h"

BuildableRenderer::BuildableRenderer(std::shared_ptr<BuildableGenerator> generator)
  :
    generator(generator)
{}

void BuildableRenderer::renderBuildable()
{
  glBindVertexArray(generator->basicGLBuffers.get(VAO));
  glEnable(GL_BLEND);
  glDrawElementsInstanced(GL_TRIANGLES, VERTICES_PER_TILE, GL_UNSIGNED_BYTE, 0, generator->numInstances);
  glDisable(GL_BLEND);
}

void BuildableRenderer::renderSelected()
{
  glBindVertexArray(generator->selectedBuffers.get(VAO));
  glEnable(GL_BLEND);
  glDrawElements(GL_TRIANGLES, VERTICES_PER_TILE, GL_UNSIGNED_BYTE, 0);
  glDisable(GL_BLEND);
}
