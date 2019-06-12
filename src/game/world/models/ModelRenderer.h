#pragma once

#include <GL/glew.h>

class BufferCollection;

class ModelRenderer
{
public:
  ModelRenderer(BufferCollection& basicGLBuffers, BufferCollection& shadowDIBO) noexcept;
  void render(bool isShadow, GLsizei primCount);
  void renderOneInstance(GLsizei numIndices);

private:
  BufferCollection& basicGLBuffers;
  BufferCollection& shadowDIBO;
};
