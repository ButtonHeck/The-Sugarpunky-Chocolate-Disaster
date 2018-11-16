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
  glm::mat4 setPos(float posDegrees);
  glm::vec3 getPosition() const;
private:
  friend class TheSunRenderer;
  void bufferData();
  OpenglBuffer basicGLBuffers;
  glm::vec3 rotateAxis;
  glm::mat4 transform;
  glm::vec3 currentPosition = glm::vec3(HALF_WORLD_WIDTH_F, 0.0f, 0.0f);

  //constants for absolute positioning
  const glm::vec3 START_POSITION = currentPosition;
  const glm::mat4 START_MODEL = glm::mat4();
};

#endif // THESUN_H
