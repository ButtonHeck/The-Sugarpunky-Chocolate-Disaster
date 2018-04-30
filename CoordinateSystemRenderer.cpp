#include "CoordinateSystemRenderer.h"

void CoordinateSystemRenderer::fillBufferData()
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  GLfloat points[] = {
      -0.9f, 0.6f, 0.0f, 0.08f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
      -0.9f, 0.6f, 0.0f, 0.0f,  0.08f, 0.0f,  0.0f, 1.0f, 0.0f,
      -0.9f, 0.6f, 0.0f, 0.0f,  0.0f,  0.08f, 0.0f, 0.0f, 1.0f
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CoordinateSystemRenderer::deleteGLObjects()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}

void CoordinateSystemRenderer::draw(Shader &shader, glm::mat4 &view, float aspect_ratio)
{
  shader.use();
  shader.setMat4("view", view);
  shader.setFloat("aspectRatio", aspect_ratio);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glDrawArrays(GL_POINTS, 0, 3);
}
