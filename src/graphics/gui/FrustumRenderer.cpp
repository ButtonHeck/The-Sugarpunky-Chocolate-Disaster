#include "FrustumRenderer.h"

FrustumRenderer::FrustumRenderer(const Frustum &frustum)
  :
    frustum(frustum),
    basicGLBuffers(VAO | VBO | EBO)
{
  basicGLBuffers.bind(VAO | VBO | EBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);
}

void FrustumRenderer::render()
{
  basicGLBuffers.bind(VAO | VBO | EBO);

  vertices[0] = frustum.nearLL.x;
  vertices[1] = frustum.nearLL.y;
  vertices[2] = frustum.nearLL.z;

  vertices[3] = frustum.nearLR.x;
  vertices[4] = frustum.nearLR.y;
  vertices[5] = frustum.nearLR.z;

  vertices[6] = frustum.nearUR.x;
  vertices[7] = frustum.nearUR.y;
  vertices[8] = frustum.nearUR.z;

  vertices[9] = frustum.nearUL.x;
  vertices[10] = frustum.nearUL.y;
  vertices[11] = frustum.nearUL.z;

  vertices[12] = frustum.farLL.x;
  vertices[13] = frustum.farLL.y;
  vertices[14] = frustum.farLL.z;

  vertices[15] = frustum.farLR.x;
  vertices[16] = frustum.farLR.y;
  vertices[17] = frustum.farLR.z;

  vertices[18] = frustum.farUR.x;
  vertices[19] = frustum.farUR.y;
  vertices[20] = frustum.farUR.z;

  vertices[21] = frustum.farUL.x;
  vertices[22] = frustum.farUL.y;
  vertices[23] = frustum.farUL.z;

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glDrawElements(GL_LINE_STRIP, 3 * Frustum::NUMBER_OF_DISTINCT_VERTICES, GL_UNSIGNED_INT, 0);
}
