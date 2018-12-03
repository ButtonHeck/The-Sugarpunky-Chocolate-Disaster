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


  vertices[12] = volume.nearBox.ll.x;
  vertices[13] = 14.0f;
  vertices[14] = volume.nearBox.ll.y;

  vertices[15] = volume.nearBox.lr.x;
  vertices[16] = 14.0f;
  vertices[17] = volume.nearBox.lr.y;

  vertices[18] = volume.nearBox.ur.x;
  vertices[19] = 14.0f;
  vertices[20] = volume.nearBox.ur.y;

  vertices[21] = volume.nearBox.ul.x;
  vertices[22] = 14.0f;
  vertices[23] = volume.nearBox.ul.y;


  vertices[24] = volume.middleBox.ll.x;
  vertices[25] = 0.001f;
  vertices[26] = volume.middleBox.ll.y;

  vertices[27] = volume.middleBox.lr.x;
  vertices[28] = 0.001f;
  vertices[29] = volume.middleBox.lr.y;

  vertices[30] = volume.middleBox.ur.x;
  vertices[31] = 0.001f;
  vertices[32] = volume.middleBox.ur.y;

  vertices[33] = volume.middleBox.ul.x;
  vertices[34] = 0.001f;
  vertices[35] = volume.middleBox.ul.y;


  vertices[36] = volume.middleBox.ll.x;
  vertices[37] = 14.0f;
  vertices[38] = volume.middleBox.ll.y;

  vertices[39] = volume.middleBox.lr.x;
  vertices[40] = 14.0f;
  vertices[41] = volume.middleBox.lr.y;

  vertices[42] = volume.middleBox.ur.x;
  vertices[43] = 14.0f;
  vertices[44] = volume.middleBox.ur.y;

  vertices[45] = volume.middleBox.ul.x;
  vertices[46] = 14.0f;
  vertices[47] = volume.middleBox.ul.y;

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  if (near)
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
  else
    glDrawElementsBaseVertex(GL_LINES, 24, GL_UNSIGNED_INT, 0, 8);
}

void ShadowVolumeRenderer::renderVolume(bool near, bool drawLightSource)
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


  volumeVertices[24] = volume.middleBox.nearLL.x;
  volumeVertices[25] = volume.middleBox.nearLL.y;
  volumeVertices[26] = volume.middleBox.nearLL.z;

  volumeVertices[27] = volume.middleBox.nearLR.x;
  volumeVertices[28] = volume.middleBox.nearLR.y;
  volumeVertices[29] = volume.middleBox.nearLR.z;

  volumeVertices[30] = volume.middleBox.nearUR.x;
  volumeVertices[31] = volume.middleBox.nearUR.y;
  volumeVertices[32] = volume.middleBox.nearUR.z;

  volumeVertices[33] = volume.middleBox.nearUL.x;
  volumeVertices[34] = volume.middleBox.nearUL.y;
  volumeVertices[35] = volume.middleBox.nearUL.z;

  volumeVertices[36] = volume.middleBox.farLL.x;
  volumeVertices[37] = volume.middleBox.farLL.y;
  volumeVertices[38] = volume.middleBox.farLL.z;

  volumeVertices[39] = volume.middleBox.farLR.x;
  volumeVertices[40] = volume.middleBox.farLR.y;
  volumeVertices[41] = volume.middleBox.farLR.z;

  volumeVertices[42] = volume.middleBox.farUR.x;
  volumeVertices[43] = volume.middleBox.farUR.y;
  volumeVertices[44] = volume.middleBox.farUR.z;

  volumeVertices[45] = volume.middleBox.farUL.x;
  volumeVertices[46] = volume.middleBox.farUL.y;
  volumeVertices[47] = volume.middleBox.farUL.z;

  volumeVertices[48] = volume.nearBox.lightSource.x;
  volumeVertices[49] = volume.nearBox.lightSource.y;
  volumeVertices[50] = volume.nearBox.lightSource.z;

  volumeVertices[51] = volume.middleBox.lightSource.x;
  volumeVertices[52] = volume.middleBox.lightSource.y;
  volumeVertices[53] = volume.middleBox.lightSource.z;

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
