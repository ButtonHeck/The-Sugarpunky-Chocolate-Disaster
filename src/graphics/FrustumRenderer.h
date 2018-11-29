#ifndef FRUSTUMRENDERER_H
#define FRUSTUMRENDERER_H
#include "graphics/Frustum.h"
#include "graphics/OpenglBuffer.h"

class FrustumRenderer
{
public:
  FrustumRenderer(Frustum& frustum);
  void render();
private:
  Frustum& frustum;
  OpenglBuffer basicGLBuffers;
  float vertices[3 * 8] = {0.0f};
  GLuint indices[36] = {0,1,2,2,3,0,
                        1,5,6,6,2,1,
                        5,4,7,7,6,5,
                        4,0,3,3,7,4,
                        3,2,6,6,7,3,
                        1,5,4,4,0,1};
};

#endif // FRUSTUMRENDERER_H
