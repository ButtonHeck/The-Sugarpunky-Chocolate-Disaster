#include "game/world/models/ModelRenderer.h"

ModelRenderer::ModelRenderer(BufferCollection &basicGLBuffers, BufferCollection &shadowDIBO)
  :
    basicGLBuffers(basicGLBuffers),
    shadowDIBO(shadowDIBO)
{}

void ModelRenderer::render(bool isShadow, GLsizei primCount)
{
  basicGLBuffers.bind(VAO);
  if (!isShadow)
    {
      BENCHMARK("Model: draw", true);
      basicGLBuffers.bind(DIBO);
      glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, primCount, 0);
    }
  else
    {
      BENCHMARK("Model: draw shadows", true);
      shadowDIBO.bind(DIBO);
      glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, primCount, 0);
    }
}

void ModelRenderer::renderOneInstance(GLsizei numIndices)
{
  basicGLBuffers.bind(VAO);
  glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}
