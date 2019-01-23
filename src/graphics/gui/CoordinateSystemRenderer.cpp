#include "graphics/gui/CoordinateSystemRenderer.h"

CoordinateSystemRenderer::CoordinateSystemRenderer(Shader *shader)
  :
    basicGLBuffers(VAO | VBO),
    shader(shader)
{
  basicGLBuffers.bind(VAO | VBO);
  constexpr GLfloat POINTS[] = {
      -0.92f, 0.58f, 0.0f, 0.08f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
      -0.92f, 0.58f, 0.0f, 0.0f,  0.08f, 0.0f,  0.0f, 1.0f, 0.0f,
      -0.92f, 0.58f, 0.0f, 0.0f,  0.0f,  0.08f, 0.0f, 0.0f, 1.0f
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(POINTS), POINTS, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CoordinateSystemRenderer::draw(const glm::mat3 &view, float aspect_ratio)
{
  glLineWidth(2);
  shader->use();
  shader->setMat3("u_view", view);
  shader->setFloat("u_aspectRatio", aspect_ratio);
  basicGLBuffers.bind(VAO);
  glDrawArrays(GL_POINTS, 0, 3);
  glLineWidth(1);
}
