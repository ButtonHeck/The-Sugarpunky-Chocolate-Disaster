#include "ShadowVolumeRenderer.h"

ShadowVolumeRenderer::ShadowVolumeRenderer(ShadowVolume &volume)
  :
    volume(volume),
    terrainSquareGLBuffers(VAO | VBO | EBO),
    volumeGLBuffers(VAO | VBO | EBO)
{
  terrainSquareGLBuffers.bind(VAO | VBO | EBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  volumeGLBuffers.bind(VAO | VBO | EBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(volumeIndices), volumeIndices, GL_STATIC_DRAW);
}

void ShadowVolumeRenderer::renderTerrainSquare(bool near)
{
  terrainSquareGLBuffers.bind(VAO | VBO | EBO);

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

void ShadowVolumeRenderer::renderVolume(bool near)
{
  volumeGLBuffers.bind(VAO | VBO | EBO);

  volumeVertices[0] = volume.nearBox.nearLL.x;
  volumeVertices[1] = volume.nearBox.nearLL.y;
  volumeVertices[2] = volume.nearBox.nearLL.z;

  volumeVertices[3] = volume.nearBox.nearLR.x;
  volumeVertices[4] = volume.nearBox.nearLR.y;
  volumeVertices[5] = volume.nearBox.nearLR.z;

  volumeVertices[6] = volume.nearBox.nearUR.x;
  volumeVertices[7] = volume.nearBox.nearUR.y;
  volumeVertices[8] = volume.nearBox.nearUR.z;

  volumeVertices[9] = volume.nearBox.nearUL.x;
  volumeVertices[10] = volume.nearBox.nearUL.y;
  volumeVertices[11] = volume.nearBox.nearUL.z;

  volumeVertices[12] = volume.nearBox.farLL.x;
  volumeVertices[13] = volume.nearBox.farLL.y;
  volumeVertices[14] = volume.nearBox.farLL.z;

  volumeVertices[15] = volume.nearBox.farLR.x;
  volumeVertices[16] = volume.nearBox.farLR.y;
  volumeVertices[17] = volume.nearBox.farLR.z;

  volumeVertices[18] = volume.nearBox.farUR.x;
  volumeVertices[19] = volume.nearBox.farUR.y;
  volumeVertices[20] = volume.nearBox.farUR.z;

  volumeVertices[21] = volume.nearBox.farUL.x;
  volumeVertices[22] = volume.nearBox.farUL.y;
  volumeVertices[23] = volume.nearBox.farUL.z;


  volumeVertices[24] = volume.farBox.nearLL.x;
  volumeVertices[25] = volume.farBox.nearLL.y;
  volumeVertices[26] = volume.farBox.nearLL.z;

  volumeVertices[27] = volume.farBox.nearLR.x;
  volumeVertices[28] = volume.farBox.nearLR.y;
  volumeVertices[29] = volume.farBox.nearLR.z;

  volumeVertices[30] = volume.farBox.nearUR.x;
  volumeVertices[31] = volume.farBox.nearUR.y;
  volumeVertices[32] = volume.farBox.nearUR.z;

  volumeVertices[33] = volume.farBox.nearUL.x;
  volumeVertices[34] = volume.farBox.nearUL.y;
  volumeVertices[35] = volume.farBox.nearUL.z;

  volumeVertices[36] = volume.farBox.farLL.x;
  volumeVertices[37] = volume.farBox.farLL.y;
  volumeVertices[38] = volume.farBox.farLL.z;

  volumeVertices[39] = volume.farBox.farLR.x;
  volumeVertices[40] = volume.farBox.farLR.y;
  volumeVertices[41] = volume.farBox.farLR.z;

  volumeVertices[42] = volume.farBox.farUR.x;
  volumeVertices[43] = volume.farBox.farUR.y;
  volumeVertices[44] = volume.farBox.farUR.z;

  volumeVertices[45] = volume.farBox.farUL.x;
  volumeVertices[46] = volume.farBox.farUL.y;
  volumeVertices[47] = volume.farBox.farUL.z;

  glBufferData(GL_ARRAY_BUFFER, sizeof(volumeVertices), volumeVertices, GL_STATIC_DRAW);
  if (near)
    glDrawElements(GL_LINE_STRIP, 24, GL_UNSIGNED_INT, 0);
  else
    glDrawElementsBaseVertex(GL_LINE_STRIP, 24, GL_UNSIGNED_INT, 0, 8);
}
