#ifndef SHADOWVOLUMEBOXRENDERER_H
#define SHADOWVOLUMEBOXRENDERER_H
#include "graphics/ShadowVolume.h"
#include "graphics/OpenglBuffer.h"

class ShadowVolumeRenderer
{
public:
  ShadowVolumeRenderer(ShadowVolume& volume);
  void render(bool near);
private:
  ShadowVolume& volume;
  OpenglBuffer basicGLBuffers;
  float vertices[24] = {0};
  GLuint indices[16] = {0,1,1,2,2,3,3,0,
                       4,5,5,6,6,7,7,4};
};

#endif // SHADOWVOLUMEBOXRENDERER_H
