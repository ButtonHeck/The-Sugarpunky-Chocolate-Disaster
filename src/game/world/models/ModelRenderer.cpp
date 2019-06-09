#include "ModelRenderer"
#include "BufferCollection"
#include "BenchmarkTimer"

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
      basicGLBuffers.bind(DIBO);
      BENCHMARK("Model: draw", true);
      glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, primCount, 0);
    }
  else
    {
      shadowDIBO.bind(DIBO);
      BENCHMARK("Model: draw shadows", true);
      glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, primCount, 0);
    }
}

void ModelRenderer::renderOneInstance(GLsizei numIndices)
{
  basicGLBuffers.bind(VAO);
  glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}
