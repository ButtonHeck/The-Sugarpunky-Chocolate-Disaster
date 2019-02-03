#ifndef LENSFLARERENDERER_H
#define LENSFLARERENDERER_H
#include <GL/glew.h>
#include "graphics/openglObjects/BufferCollection.h"

class LensFlareRenderer
{
public:
  LensFlareRenderer(BufferCollection& basicGLBuffer);
  void draw(unsigned int numFlares);

private:
  BufferCollection& basicGLBuffer;
};

#endif // LENSFLARERENDERER_H
