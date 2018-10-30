#include "TheSun.h"

TheSun::TheSun()
  :
    basicGLBuffers(VAO | VBO | TFBO),
    rotateAxis(0.0f, 0.0f, 1.0f)
{
  bufferData();
}

glm::mat4 TheSun::move(float delta)
{
  return glm::rotate(transform, glm::radians(delta), rotateAxis);
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
