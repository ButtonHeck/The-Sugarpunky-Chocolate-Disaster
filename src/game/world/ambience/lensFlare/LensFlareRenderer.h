#ifndef LENSFLARERENDERER_H
#define LENSFLARERENDERER_H
#include <GL/glew.h>
#include "graphics/OpenglBuffer.h"

class LensFlareRenderer
{
public:
  LensFlareRenderer(OpenglBuffer& basicGLBuffer);
  void draw(unsigned int numFlares);
private:
  OpenglBuffer& basicGLBuffer;
};

#endif // LENSFLARERENDERER_H
