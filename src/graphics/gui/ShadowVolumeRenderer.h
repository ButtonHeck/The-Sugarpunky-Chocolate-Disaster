#ifndef SHADOWVOLUMEBOXRENDERER_H
#define SHADOWVOLUMEBOXRENDERER_H
#include "graphics/ShadowVolume.h"
#include "graphics/openglObjects/BufferCollection.h"

class ShadowVolumeRenderer
{
public:
  ShadowVolumeRenderer(ShadowVolume& shadowVolume);
  void bufferExpectedVolumes();
  void renderExpectedVolume(int index);
  void bufferActualVolumes();
  void renderActualVolume(int index);
  void renderLightSource(int index);
private:
  ShadowVolume& shadowVolume;
  BufferCollection expectedVolumesGLBuffers;
  float expectedVolumesVertices[72] = {0};
  GLuint expectedVolumesIndices[24] = {0,1,1,2,2,3,3,0,
                                       4,5,5,6,6,7,7,4,
                                       0,4,1,5,2,6,3,7};
  BufferCollection actualVolumesGLBuffers;
  float actualVolumeVertices[81] = {0};
  GLuint actualVolumeIndices[36] = {0,1,2,2,3,0,
                                    1,5,6,6,2,1,
                                    5,4,7,7,6,5,
                                    4,0,3,3,7,4,
                                    3,2,6,6,7,3,
                                    1,5,4,4,0,1};
};

#endif // SHADOWVOLUMEBOXRENDERER_H
