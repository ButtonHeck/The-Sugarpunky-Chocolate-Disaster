#ifndef SHADOWVOLUMEBOXRENDERER_H
#define SHADOWVOLUMEBOXRENDERER_H
#include "graphics/ShadowVolume.h"
#include "graphics/OpenglBuffer.h"

class ShadowVolumeRenderer
{
public:
  ShadowVolumeRenderer(ShadowVolume& volume);
  void renderTerrainSquare(bool near);
  void renderVolume(bool near, bool drawLightSource);
private:
  ShadowVolume& volume;
  OpenglBuffer terrainSquareGLBuffers;
  float vertices[24] = {0};
  GLuint indices[16] = {0,1,1,2,2,3,3,0,
                       4,5,5,6,6,7,7,4};
  OpenglBuffer volumeGLBuffers;
  float volumeVertices[54] = {0};
  GLuint volumeIndices[36] = {0,1,2,2,3,0,
                             1,5,6,6,2,1,
                             5,4,7,7,6,5,
                             4,0,3,3,7,4,
                             3,2,6,6,7,3,
                             1,5,4,4,0,1};
};

#endif // SHADOWVOLUMEBOXRENDERER_H
