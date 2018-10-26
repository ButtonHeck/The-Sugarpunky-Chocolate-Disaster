#ifndef THESUN_H
#define THESUN_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/OpenglBuffer.h"

class TheSun
{
public:
  TheSun();
  glm::mat4 move(float delta);
  GLuint getVAO();
private:
  void bufferData();
  OpenglBuffer basicGLBuffers;
  glm::vec3 rotateAxis;
  glm::mat4 transform;
};

#endif // THESUN_H
