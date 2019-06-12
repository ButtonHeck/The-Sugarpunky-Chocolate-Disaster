#include "ShoreRenderer"
#include "ShoreGenerator"

ShoreRenderer::ShoreRenderer(ShoreGenerator &generator) noexcept
  :
    generator(generator)
{}

void ShoreRenderer::render()
{
  generator.basicGLBuffers.bind(VAO);
  glEnable(GL_BLEND);
  glDrawElements(GL_TRIANGLES, generator.tiles.size() * VERTICES_PER_QUAD, GL_UNSIGNED_INT, 0);
  glDisable(GL_BLEND);
}

void ShoreRenderer::debugRender(GLenum primitiveType)
{
  generator.basicGLBuffers.bind(VAO);
  glLineWidth(2.0f);
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDrawElements(primitiveType, generator.tiles.size() * VERTICES_PER_QUAD, GL_UNSIGNED_INT, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);
  glLineWidth(1.0f);
}
