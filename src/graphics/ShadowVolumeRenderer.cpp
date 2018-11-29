#include "ShadowVolumeRenderer.h"

ShadowVolumeRenderer::ShadowVolumeRenderer(ShadowVolume &volume)
  :
    volume(volume),
    basicGLBuffers(VAO | VBO | EBO)
{
  basicGLBuffers.bind(VAO | VBO | EBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void ShadowVolumeRenderer::render(bool near)
{
  basicGLBuffers.bind(VAO | VBO | EBO);

  vertices[0] = volume.nearBox.ll.x;
  vertices[1] = 0.001f;
  vertices[2] = volume.nearBox.ll.y;

  vertices[3] = volume.nearBox.lr.x;
  vertices[4] = 0.001f;
  vertices[5] = volume.nearBox.lr.y;

  vertices[6] = volume.nearBox.ur.x;
  vertices[7] = 0.001f;
  vertices[8] = volume.nearBox.ur.y;

  vertices[9] = volume.nearBox.ul.x;
  vertices[10] = 0.001f;
  vertices[11] = volume.nearBox.ul.y;

  vertices[12] = volume.farBox.ll.x;
  vertices[13] = 0.001f;
  vertices[14] = volume.farBox.ll.y;

  vertices[15] = volume.farBox.lr.x;
  vertices[16] = 0.001f;
  vertices[17] = volume.farBox.lr.y;

  vertices[18] = volume.farBox.ur.x;
  vertices[19] = 0.001f;
  vertices[20] = volume.farBox.ur.y;

  vertices[21] = volume.farBox.ul.x;
  vertices[22] = 0.001f;
  vertices[23] = volume.farBox.ul.y;

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  if (near)
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
  else
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (void*)(8 * sizeof(GLuint)));
}
