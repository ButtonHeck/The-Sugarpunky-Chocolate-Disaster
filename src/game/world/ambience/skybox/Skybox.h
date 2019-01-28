#ifndef SKYBOX_H
#define SKYBOX_H
#include <GL/glew.h>
#include "graphics/openglObjects/BufferCollection.h"

class Skybox
{
public:
  Skybox();
  virtual ~Skybox() = default;
  static constexpr int BUFFER_SIZE_FLOATS = 3 * 6 * 6;
private:
  friend class SkyboxRenderer;
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
