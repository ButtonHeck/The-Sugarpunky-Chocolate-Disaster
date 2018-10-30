#include "game/world/ambience/skybox/Skybox.h"

Skybox::Skybox()
  :
    basicGLBuffers(VAO | VBO)
{
  basicGLBuffers.bind(VAO | VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NUM_POSITIONS, &positions[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
