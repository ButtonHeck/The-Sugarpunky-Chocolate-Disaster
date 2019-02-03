#ifndef SKYBOX_H
#define SKYBOX_H
#include <GL/glew.h>
#include "graphics/openglObjects/BufferCollection.h"

class Skybox
{
public:
  Skybox();
  virtual ~Skybox() = default;
  constexpr static unsigned int BUFFER_SIZE_FLOATS = 3 * 6 * 6;
  constexpr static unsigned int VERTICES_PER_SKYBOX = VERTICES_PER_QUAD * VERTICES_PER_QUAD;
  void draw();

private:
  BufferCollection basicGLBuffers;
  GLfloat vertices[BUFFER_SIZE_FLOATS] = {
    -4.0f,  1.2f, -4.0f,
    -4.0f, -0.8f, -4.0f,
     4.0f, -0.8f, -4.0f,
     4.0f, -0.8f, -4.0f,
     4.0f,  1.2f, -4.0f,
    -4.0f,  1.2f, -4.0f,

    -4.0f, -0.8f,  4.0f,
    -4.0f, -0.8f, -4.0f,
    -4.0f,  1.2f, -4.0f,
    -4.0f,  1.2f, -4.0f,
    -4.0f,  1.2f,  4.0f,
    -4.0f, -0.8f,  4.0f,

     4.0f, -0.8f, -4.0f,
     4.0f, -0.8f,  4.0f,
     4.0f,  1.2f,  4.0f,
     4.0f,  1.2f,  4.0f,
     4.0f,  1.2f, -4.0f,
     4.0f, -0.8f, -4.0f,

    -4.0f, -0.8f,  4.0f,
    -4.0f,  1.2f,  4.0f,
     4.0f,  1.2f,  4.0f,
     4.0f,  1.2f,  4.0f,
     4.0f, -0.8f,  4.0f,
    -4.0f, -0.8f,  4.0f,

    -4.0f,  1.2f, -4.0f,
     4.0f,  1.2f, -4.0f,
     4.0f,  1.2f,  4.0f,
     4.0f,  1.2f,  4.0f,
    -4.0f,  1.2f,  4.0f,
    -4.0f,  1.2f, -4.0f,

    -4.0f, -0.8f, -4.0f,
    -4.0f, -0.8f,  4.0f,
     4.0f, -0.8f, -4.0f,
     4.0f, -0.8f, -4.0f,
    -4.0f, -0.8f,  4.0f,
     4.0f, -0.8f,  4.0f
  };
};

#endif // SKYBOX_H
