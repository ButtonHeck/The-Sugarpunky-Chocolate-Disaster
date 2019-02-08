#include "game/world/ambience/theSun/TheSun.h"

TheSun::TheSun()
  :
    ROTATION_VECTOR(0.0f, 0.0f, 1.0f),
    basicGLBuffers(VAO | VBO),
    currentPosition(HALF_WORLD_WIDTH_F, 0.0f, 0.0f)
{
  basicGLBuffers.bind(VAO | VBO);
  GLfloat vertices[4] = {1.0f, 0.0f, 0.0f, 1.0f};
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  BufferCollection::bindZero(VAO | VBO);
}

void TheSun::move(float angleDegrees)
{
  currentPosition = glm::rotate(currentPosition, glm::radians(angleDegrees), ROTATION_VECTOR);
  rotationTransform = glm::rotate(rotationTransform, glm::radians(angleDegrees), ROTATION_VECTOR);
  lightDirTo = glm::normalize(-currentPosition);
}

void TheSun::moveAbsolutePosition(float angleDegrees)
{
  currentPosition = glm::rotate(START_POSITION, glm::radians(angleDegrees), ROTATION_VECTOR);
  rotationTransform = glm::rotate(START_MODEL, glm::radians(angleDegrees), ROTATION_VECTOR);
  lightDirTo = glm::normalize(-currentPosition);
}

const glm::vec3& TheSun::getPosition() const
{
  return currentPosition;
}

const glm::vec3 &TheSun::getLightDir() const
{
  return lightDirTo;
}

const glm::mat4 &TheSun::getRotationTransform() const
{
  return rotationTransform;
}
