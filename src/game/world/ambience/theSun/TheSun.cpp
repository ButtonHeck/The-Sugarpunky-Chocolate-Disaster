#include "game/world/ambience/theSun/TheSun.h"

TheSun::TheSun()
  :
    basicGLBuffers(VAO | VBO | TFBO),
    rotateAxis(0.0f, 0.0f, 1.0f)
{
  bufferData();
}

glm::mat4 TheSun::move(float delta)
{
  currentPosition = glm::rotateZ(currentPosition, glm::radians(delta));
  transform = glm::rotate(transform, glm::radians(delta), rotateAxis);
  return transform;
}

glm::mat4 TheSun::setPos(float posDegrees)
{
  currentPosition = glm::rotateZ(START_POSITION, glm::radians(posDegrees));
  transform = glm::rotate(START_MODEL, glm::radians(posDegrees), rotateAxis);
  return transform;
}

glm::vec3 TheSun::getPosition() const
{
  return currentPosition;
}

void TheSun::bufferData()
{
  basicGLBuffers.bind(VAO | VBO);
  GLfloat vertices[4] = {1.0f, 0.0f, 0.0f, 1.0f};
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
