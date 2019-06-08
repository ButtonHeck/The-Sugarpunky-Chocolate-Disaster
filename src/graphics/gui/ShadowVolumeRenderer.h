#ifndef SHADOWVOLUMEBOXRENDERER_H
#define SHADOWVOLUMEBOXRENDERER_H
#include "ShadowVolume"
#include "BufferCollection"

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
  constexpr static unsigned int VOLUMES_INDICES_COUNT = 24;
  constexpr static float LIGHT_SOURCE_POSITION_POINT_SIZE = 12.0f;

  ShadowVolume& shadowVolume;
  BufferCollection expectedVolumesGLBuffers;
  /* 3 stands for number of floats (x,y,z) of each vertex
   * and we need them twice because expected vertices duplicates and translates to ShadowVolume::BOX_MIN_HEIGHT in y-axis
   * to form the box itself
   */
  GLfloat expectedVolumesVertices[NUM_SHADOW_LAYERS * ShadowVolume::BOX_EXPECTED_VERTICES * 3 * 2] = {0};

  BufferCollection actualVolumesGLBuffers;
  /* +1 stands for one vertex of a sun position(relative to the box itself)
   * and 3 is again - the number of floats of each vertex
   */
  GLfloat actualVolumeVertices[NUM_SHADOW_LAYERS * (ShadowVolume::BOX_ACTUAL_VERTICES + 1) * 3] = {0};

  GLuint volumesIndices[VOLUMES_INDICES_COUNT] = {0,1,2,2,3,0,
                                                  1,5,6,6,2,1,
                                                  5,4,7,7,6,5,
                                                  4,0,3,3,7,4};
};

#endif // SHADOWVOLUMEBOXRENDERER_H
