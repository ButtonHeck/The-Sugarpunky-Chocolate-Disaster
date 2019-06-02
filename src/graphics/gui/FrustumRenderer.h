#ifndef FRUSTUMRENDERER_H
#define FRUSTUMRENDERER_H
#include "../../src/graphics/Frustum.h"
#include "../../src/graphics/openglObjects/BufferCollection.h"

class FrustumRenderer
{
public:
  FrustumRenderer(const Frustum& frustum);
  void render();

private:
  const GLuint INDICES[Frustum::NUMBER_OF_PLANES * VERTICES_PER_QUAD] =
    {0,1,2,2,3,0,
     1,5,6,6,2,1,
     5,4,7,7,6,5,
     4,0,3,3,7,4,
     3,2,6,6,7,3,
     1,5,4,4,0,1};

  const Frustum& frustum;
  BufferCollection basicGLBuffers;
  float vertices[3 * Frustum::NUMBER_OF_DISTINCT_VERTICES] = {0.0f};
};

#endif // FRUSTUMRENDERER_H
