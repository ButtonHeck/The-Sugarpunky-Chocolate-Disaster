#include "game/world/ambience/skybox/Skybox.h"

Skybox::Skybox()
  :
    basicGLBuffers(VAO | VBO)
{
  basicGLBuffers.bind(VAO | VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  BufferCollection::bindZero(VAO | VBO);
}

void Skybox::draw()
{
  basicGLBuffers.bind(VAO);
  glDrawArrays(GL_TRIANGLES, 0, VERTICES_PER_SKYBOX);
}
