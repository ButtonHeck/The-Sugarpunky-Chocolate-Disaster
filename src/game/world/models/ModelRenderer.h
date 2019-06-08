#pragma once

#include "BufferCollection"
#include "BenchmarkTimer"

#include <GL/glew.h>

class ModelRenderer
{
public:
  ModelRenderer(BufferCollection& basicGLBuffers, BufferCollection& shadowDIBO);
  void render(bool isShadow, GLsizei primCount);
  void renderOneInstance(GLsizei numIndices);

private:
  BufferCollection& basicGLBuffers;
  BufferCollection& shadowDIBO;
};
