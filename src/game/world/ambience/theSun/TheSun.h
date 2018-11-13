#ifndef THESUN_H
#define THESUN_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "graphics/OpenglBuffer.h"
#include "util/Settings.h"

class TheSun
{
public:
  TheSun();
  glm::mat4 move(float delta);
  glm::vec3 getPosition() const;
private:
  friend class TheSunRenderer;
  void bufferData();
  OpenglBuffer basicGLBuffers;
  glm::vec3 rotateAxis;
  glm::mat4 transform;
  glm::vec3 currentPosition;
  const glm::vec3 START_POSITION = glm::vec3(HALF_WORLD_WIDTH_F, 0.0f, 0.0f);
};

#endif // THESUN_H
