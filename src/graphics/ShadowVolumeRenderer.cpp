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

  vertices[0] = volume.shadowBoxes[0].ll.x;
  vertices[1] = 0.001f;
  vertices[2] = volume.shadowBoxes[0].ll.y;

  vertices[3] = volume.shadowBoxes[0].lr.x;
  vertices[4] = 0.001f;
  vertices[5] = volume.shadowBoxes[0].lr.y;

  vertices[6] = volume.shadowBoxes[0].ur.x;
  vertices[7] = 0.001f;
  vertices[8] = volume.shadowBoxes[0].ur.y;

  vertices[9] = volume.shadowBoxes[0].ul.x;
  vertices[10] = 0.001f;
  vertices[11] = volume.shadowBoxes[0].ul.y;


  vertices[12] = volume.shadowBoxes[0].ll.x;
  vertices[13] = 14.0f;
  vertices[14] = volume.shadowBoxes[0].ll.y;

  vertices[15] = volume.shadowBoxes[0].lr.x;
  vertices[16] = 14.0f;
  vertices[17] = volume.shadowBoxes[0].lr.y;

  vertices[18] = volume.shadowBoxes[0].ur.x;
  vertices[19] = 14.0f;
  vertices[20] = volume.shadowBoxes[0].ur.y;

  vertices[21] = volume.shadowBoxes[0].ul.x;
  vertices[22] = 14.0f;
  vertices[23] = volume.shadowBoxes[0].ul.y;


  vertices[24] = volume.shadowBoxes[1].ll.x;
  vertices[25] = 0.001f;
  vertices[26] = volume.shadowBoxes[1].ll.y;

  vertices[27] = volume.shadowBoxes[1].lr.x;
  vertices[28] = 0.001f;
  vertices[29] = volume.shadowBoxes[1].lr.y;

  vertices[30] = volume.shadowBoxes[1].ur.x;
  vertices[31] = 0.001f;
  vertices[32] = volume.shadowBoxes[1].ur.y;

  vertices[33] = volume.shadowBoxes[1].ul.x;
  vertices[34] = 0.001f;
  vertices[35] = volume.shadowBoxes[1].ul.y;


  vertices[36] = volume.shadowBoxes[1].ll.x;
  vertices[37] = 14.0f;
  vertices[38] = volume.shadowBoxes[1].ll.y;

  vertices[39] = volume.shadowBoxes[1].lr.x;
  vertices[40] = 14.0f;
  vertices[41] = volume.shadowBoxes[1].lr.y;

  vertices[42] = volume.shadowBoxes[1].ur.x;
  vertices[43] = 14.0f;
  vertices[44] = volume.shadowBoxes[1].ur.y;

  vertices[45] = volume.shadowBoxes[1].ul.x;
  vertices[46] = 14.0f;
  vertices[47] = volume.shadowBoxes[1].ul.y;

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  if (near)
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
  else
    glDrawElementsBaseVertex(GL_LINES, 24, GL_UNSIGNED_INT, 0, 8);
}

void ShadowVolumeRenderer::renderVolume(bool near, bool drawLightSource)
{
  volumeGLBuffers.bind(VAO | VBO | EBO);

  volumeVertices[0] = volume.shadowBoxes[0].nearLL.x;
  volumeVertices[1] = volume.shadowBoxes[0].nearLL.y;
  volumeVertices[2] = volume.shadowBoxes[0].nearLL.z;

  volumeVertices[3] = volume.shadowBoxes[0].nearLR.x;
  volumeVertices[4] = volume.shadowBoxes[0].nearLR.y;
  volumeVertices[5] = volume.shadowBoxes[0].nearLR.z;

  volumeVertices[6] = volume.shadowBoxes[0].nearUR.x;
  volumeVertices[7] = volume.shadowBoxes[0].nearUR.y;
  volumeVertices[8] = volume.shadowBoxes[0].nearUR.z;

  volumeVertices[9] = volume.shadowBoxes[0].nearUL.x;
  volumeVertices[10] = volume.shadowBoxes[0].nearUL.y;
  volumeVertices[11] = volume.shadowBoxes[0].nearUL.z;

  volumeVertices[12] = volume.shadowBoxes[0].farLL.x;
  volumeVertices[13] = volume.shadowBoxes[0].farLL.y;
  volumeVertices[14] = volume.shadowBoxes[0].farLL.z;

  volumeVertices[15] = volume.shadowBoxes[0].farLR.x;
  volumeVertices[16] = volume.shadowBoxes[0].farLR.y;
  volumeVertices[17] = volume.shadowBoxes[0].farLR.z;

  volumeVertices[18] = volume.shadowBoxes[0].farUR.x;
  volumeVertices[19] = volume.shadowBoxes[0].farUR.y;
  volumeVertices[20] = volume.shadowBoxes[0].farUR.z;

  volumeVertices[21] = volume.shadowBoxes[0].farUL.x;
  volumeVertices[22] = volume.shadowBoxes[0].farUL.y;
  volumeVertices[23] = volume.shadowBoxes[0].farUL.z;


  volumeVertices[24] = volume.shadowBoxes[1].nearLL.x;
  volumeVertices[25] = volume.shadowBoxes[1].nearLL.y;
  volumeVertices[26] = volume.shadowBoxes[1].nearLL.z;

  volumeVertices[27] = volume.shadowBoxes[1].nearLR.x;
  volumeVertices[28] = volume.shadowBoxes[1].nearLR.y;
  volumeVertices[29] = volume.shadowBoxes[1].nearLR.z;

  volumeVertices[30] = volume.shadowBoxes[1].nearUR.x;
  volumeVertices[31] = volume.shadowBoxes[1].nearUR.y;
  volumeVertices[32] = volume.shadowBoxes[1].nearUR.z;

  volumeVertices[33] = volume.shadowBoxes[1].nearUL.x;
  volumeVertices[34] = volume.shadowBoxes[1].nearUL.y;
  volumeVertices[35] = volume.shadowBoxes[1].nearUL.z;

  volumeVertices[36] = volume.shadowBoxes[1].farLL.x;
  volumeVertices[37] = volume.shadowBoxes[1].farLL.y;
  volumeVertices[38] = volume.shadowBoxes[1].farLL.z;

  volumeVertices[39] = volume.shadowBoxes[1].farLR.x;
  volumeVertices[40] = volume.shadowBoxes[1].farLR.y;
  volumeVertices[41] = volume.shadowBoxes[1].farLR.z;

  volumeVertices[42] = volume.shadowBoxes[1].farUR.x;
  volumeVertices[43] = volume.shadowBoxes[1].farUR.y;
  volumeVertices[44] = volume.shadowBoxes[1].farUR.z;

  volumeVertices[45] = volume.shadowBoxes[1].farUL.x;
  volumeVertices[46] = volume.shadowBoxes[1].farUL.y;
  volumeVertices[47] = volume.shadowBoxes[1].farUL.z;

  volumeVertices[48] = volume.shadowBoxes[0].lightSource.x;
  volumeVertices[49] = volume.shadowBoxes[0].lightSource.y;
  volumeVertices[50] = volume.shadowBoxes[0].lightSource.z;

  volumeVertices[51] = volume.shadowBoxes[1].lightSource.x;
  volumeVertices[52] = volume.shadowBoxes[1].lightSource.y;
  volumeVertices[53] = volume.shadowBoxes[1].lightSource.z;

  glBufferData(GL_ARRAY_BUFFER, sizeof(volumeVertices), volumeVertices, GL_STATIC_DRAW);
  if (drawLightSource)
    {
      glPointSize(12.0f);
      if (near)
        glDrawElementsBaseVertex(GL_POINTS, 1, GL_UNSIGNED_INT, 0, 16);
      else
        glDrawElementsBaseVertex(GL_POINTS, 1, GL_UNSIGNED_INT, 0, 17);
    }
  else
    {
      if (near)
        glDrawElements(GL_LINE_STRIP, 24, GL_UNSIGNED_INT, 0);
      else
        glDrawElementsBaseVertex(GL_LINE_STRIP, 24, GL_UNSIGNED_INT, 0, 8);
    }
}
