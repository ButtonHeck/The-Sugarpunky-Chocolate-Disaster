#ifndef SKYBOX_H
#define SKYBOX_H
#include <GL/glew.h>
#include "graphics/OpenglBuffer.h"

class Skybox
{
public:
  Skybox();
  virtual ~Skybox() = default;
  static constexpr int NUM_POSITIONS = 108;
private:
  friend class SkyboxRenderer;
  OpenglBuffer basicGLBuffers;
  GLfloat positions[NUM_POSITIONS] = {
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
