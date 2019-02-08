#ifndef THESUN_H
#define THESUN_H
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "graphics/openglObjects/BufferCollection.h"
#include "game/world/SceneSettings.h"

class TheSun
{
public:
  TheSun();
  void move(float angleDegrees);
  void moveAbsolutePosition(float angleDegrees);
  const glm::vec3 &getPosition() const;
  const glm::vec3 &getLightDir() const;
  const glm::mat4 &getRotationTransform() const;

private:
  friend class TheSunRenderer;
  const glm::vec3 ROTATION_VECTOR;
  BufferCollection basicGLBuffers;
  glm::mat4 rotationTransform;
  glm::vec3 currentPosition;
  glm::vec3 lightDirTo;

  //constants for absolute positioning
  const glm::vec3 START_POSITION = currentPosition;
  const glm::mat4 START_MODEL = glm::mat4();
};

#endif // THESUN_H
