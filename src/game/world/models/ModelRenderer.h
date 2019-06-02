#ifndef MODELRENDERER_H
#define MODELRENDERER_H
#include <GL/glew.h>
#include "../../src/graphics/openglObjects/BufferCollection.h"
#include "../../src/util/BenchmarkTimer.h"

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

#endif // MODELRENDERER_H
