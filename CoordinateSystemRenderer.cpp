#include "CoordinateSystemRenderer.h"

CoordinateSystemRenderer::CoordinateSystemRenderer(Shader *shader)
  :
    shader(shader)
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  GLfloat points[] = {
      -0.92f, 0.58f, 0.0f, 0.08f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
      -0.92f, 0.58f, 0.0f, 0.0f,  0.08f, 0.0f,  0.0f, 1.0f, 0.0f,
      -0.92f, 0.58f, 0.0f, 0.0f,  0.0f,  0.08f, 0.0f, 0.0f, 1.0f
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

void CoordinateSystemRenderer::draw(const glm::mat4 &view, float aspect_ratio)
{
  glLineWidth(2);
  shader->use();
  shader->setMat4("u_view", view);
  shader->setFloat("u_aspectRatio", aspect_ratio);
  glBindVertexArray(vao);
  glDrawArrays(GL_POINTS, 0, 3);
  glLineWidth(1);
}
