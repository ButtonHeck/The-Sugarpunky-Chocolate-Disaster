#include "ShadowVolumeRenderer.h"

ShadowVolumeRenderer::ShadowVolumeRenderer(ShadowVolume &shadowVolume)
  :
    shadowVolume(shadowVolume),
    expectedVolumesGLBuffers(VAO | VBO | EBO),
    actualVolumesGLBuffers(VAO | VBO | EBO)
{
  expectedVolumesGLBuffers.bind(VAO | VBO | EBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(expectedVolumesIndices), expectedVolumesIndices, GL_STATIC_DRAW);

  actualVolumesGLBuffers.bind(VAO | VBO | EBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(actualVolumeIndices), actualVolumeIndices, GL_STATIC_DRAW);
}

void ShadowVolumeRenderer::bufferExpectedVolumes()
{
  //near box 0.0 for Y
  expectedVolumesVertices[0] = shadowVolume.shadowBoxes[0].ll.x;
  expectedVolumesVertices[1] = 0.001f;
  expectedVolumesVertices[2] = shadowVolume.shadowBoxes[0].ll.y;

  expectedVolumesVertices[3] = shadowVolume.shadowBoxes[0].lr.x;
  expectedVolumesVertices[4] = 0.001f;
  expectedVolumesVertices[5] = shadowVolume.shadowBoxes[0].lr.y;

  expectedVolumesVertices[6] = shadowVolume.shadowBoxes[0].ur.x;
  expectedVolumesVertices[7] = 0.001f;
  expectedVolumesVertices[8] = shadowVolume.shadowBoxes[0].ur.y;

  expectedVolumesVertices[9] = shadowVolume.shadowBoxes[0].ul.x;
  expectedVolumesVertices[10] = 0.001f;
  expectedVolumesVertices[11] = shadowVolume.shadowBoxes[0].ul.y;

  //near box 14.0 for Y
  expectedVolumesVertices[12] = shadowVolume.shadowBoxes[0].ll.x;
  expectedVolumesVertices[13] = 14.0f;
  expectedVolumesVertices[14] = shadowVolume.shadowBoxes[0].ll.y;

  expectedVolumesVertices[15] = shadowVolume.shadowBoxes[0].lr.x;
  expectedVolumesVertices[16] = 14.0f;
  expectedVolumesVertices[17] = shadowVolume.shadowBoxes[0].lr.y;

  expectedVolumesVertices[18] = shadowVolume.shadowBoxes[0].ur.x;
  expectedVolumesVertices[19] = 14.0f;
  expectedVolumesVertices[20] = shadowVolume.shadowBoxes[0].ur.y;

  expectedVolumesVertices[21] = shadowVolume.shadowBoxes[0].ul.x;
  expectedVolumesVertices[22] = 14.0f;
  expectedVolumesVertices[23] = shadowVolume.shadowBoxes[0].ul.y;

  //middle box 0.0 for Y
  expectedVolumesVertices[24] = shadowVolume.shadowBoxes[1].ll.x;
  expectedVolumesVertices[25] = 0.001f;
  expectedVolumesVertices[26] = shadowVolume.shadowBoxes[1].ll.y;

  expectedVolumesVertices[27] = shadowVolume.shadowBoxes[1].lr.x;
  expectedVolumesVertices[28] = 0.001f;
  expectedVolumesVertices[29] = shadowVolume.shadowBoxes[1].lr.y;

  expectedVolumesVertices[30] = shadowVolume.shadowBoxes[1].ur.x;
  expectedVolumesVertices[31] = 0.001f;
  expectedVolumesVertices[32] = shadowVolume.shadowBoxes[1].ur.y;

  expectedVolumesVertices[33] = shadowVolume.shadowBoxes[1].ul.x;
  expectedVolumesVertices[34] = 0.001f;
  expectedVolumesVertices[35] = shadowVolume.shadowBoxes[1].ul.y;

  //middle box 14.0 for Y
  expectedVolumesVertices[36] = shadowVolume.shadowBoxes[1].ll.x;
  expectedVolumesVertices[37] = 14.0f;
  expectedVolumesVertices[38] = shadowVolume.shadowBoxes[1].ll.y;

  expectedVolumesVertices[39] = shadowVolume.shadowBoxes[1].lr.x;
  expectedVolumesVertices[40] = 14.0f;
  expectedVolumesVertices[41] = shadowVolume.shadowBoxes[1].lr.y;

  expectedVolumesVertices[42] = shadowVolume.shadowBoxes[1].ur.x;
  expectedVolumesVertices[43] = 14.0f;
  expectedVolumesVertices[44] = shadowVolume.shadowBoxes[1].ur.y;

  expectedVolumesVertices[45] = shadowVolume.shadowBoxes[1].ul.x;
  expectedVolumesVertices[46] = 14.0f;
  expectedVolumesVertices[47] = shadowVolume.shadowBoxes[1].ul.y;

  //far box 0.0 for Y
  expectedVolumesVertices[48] = shadowVolume.shadowBoxes[2].ll.x;
  expectedVolumesVertices[49] = 0.001f;
  expectedVolumesVertices[50] = shadowVolume.shadowBoxes[2].ll.y;

  expectedVolumesVertices[51] = shadowVolume.shadowBoxes[2].lr.x;
  expectedVolumesVertices[52] = 0.001f;
  expectedVolumesVertices[53] = shadowVolume.shadowBoxes[2].lr.y;

  expectedVolumesVertices[54] = shadowVolume.shadowBoxes[2].ur.x;
  expectedVolumesVertices[55] = 0.001f;
  expectedVolumesVertices[56] = shadowVolume.shadowBoxes[2].ur.y;

  expectedVolumesVertices[57] = shadowVolume.shadowBoxes[2].ul.x;
  expectedVolumesVertices[58] = 0.001f;
  expectedVolumesVertices[59] = shadowVolume.shadowBoxes[2].ul.y;

  //far box 14.0 for Y
  expectedVolumesVertices[60] = shadowVolume.shadowBoxes[2].ll.x;
  expectedVolumesVertices[61] = 14.0f;
  expectedVolumesVertices[62] = shadowVolume.shadowBoxes[2].ll.y;

  expectedVolumesVertices[63] = shadowVolume.shadowBoxes[2].lr.x;
  expectedVolumesVertices[64] = 14.0f;
  expectedVolumesVertices[65] = shadowVolume.shadowBoxes[2].lr.y;

  expectedVolumesVertices[66] = shadowVolume.shadowBoxes[2].ur.x;
  expectedVolumesVertices[67] = 14.0f;
  expectedVolumesVertices[68] = shadowVolume.shadowBoxes[2].ur.y;

  expectedVolumesVertices[69] = shadowVolume.shadowBoxes[2].ul.x;
  expectedVolumesVertices[70] = 14.0f;
  expectedVolumesVertices[71] = shadowVolume.shadowBoxes[2].ul.y;

  expectedVolumesGLBuffers.bind(VAO | VBO | EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(expectedVolumesVertices), expectedVolumesVertices, GL_STATIC_DRAW);
}

void ShadowVolumeRenderer::renderExpectedVolume(int index)
{
  expectedVolumesGLBuffers.bind(VAO | VBO | EBO);
  if (index == 0)
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
  else if (index == 1)
    glDrawElementsBaseVertex(GL_LINES, 24, GL_UNSIGNED_INT, 0, 8);
  else if (index == 2)
    glDrawElementsBaseVertex(GL_LINES, 24, GL_UNSIGNED_INT, 0, 16);
}

void ShadowVolumeRenderer::bufferActualVolumes()
{
  //near box
  actualVolumeVertices[0] = shadowVolume.shadowBoxes[0].nearLL.x;
  actualVolumeVertices[1] = shadowVolume.shadowBoxes[0].nearLL.y;
  actualVolumeVertices[2] = shadowVolume.shadowBoxes[0].nearLL.z;

  actualVolumeVertices[3] = shadowVolume.shadowBoxes[0].nearLR.x;
  actualVolumeVertices[4] = shadowVolume.shadowBoxes[0].nearLR.y;
  actualVolumeVertices[5] = shadowVolume.shadowBoxes[0].nearLR.z;

  actualVolumeVertices[6] = shadowVolume.shadowBoxes[0].nearUR.x;
  actualVolumeVertices[7] = shadowVolume.shadowBoxes[0].nearUR.y;
  actualVolumeVertices[8] = shadowVolume.shadowBoxes[0].nearUR.z;

  actualVolumeVertices[9] = shadowVolume.shadowBoxes[0].nearUL.x;
  actualVolumeVertices[10] = shadowVolume.shadowBoxes[0].nearUL.y;
  actualVolumeVertices[11] = shadowVolume.shadowBoxes[0].nearUL.z;

  actualVolumeVertices[12] = shadowVolume.shadowBoxes[0].farLL.x;
  actualVolumeVertices[13] = shadowVolume.shadowBoxes[0].farLL.y;
  actualVolumeVertices[14] = shadowVolume.shadowBoxes[0].farLL.z;

  actualVolumeVertices[15] = shadowVolume.shadowBoxes[0].farLR.x;
  actualVolumeVertices[16] = shadowVolume.shadowBoxes[0].farLR.y;
  actualVolumeVertices[17] = shadowVolume.shadowBoxes[0].farLR.z;

  actualVolumeVertices[18] = shadowVolume.shadowBoxes[0].farUR.x;
  actualVolumeVertices[19] = shadowVolume.shadowBoxes[0].farUR.y;
  actualVolumeVertices[20] = shadowVolume.shadowBoxes[0].farUR.z;

  actualVolumeVertices[21] = shadowVolume.shadowBoxes[0].farUL.x;
  actualVolumeVertices[22] = shadowVolume.shadowBoxes[0].farUL.y;
  actualVolumeVertices[23] = shadowVolume.shadowBoxes[0].farUL.z;

  //middle box
  actualVolumeVertices[24] = shadowVolume.shadowBoxes[1].nearLL.x;
  actualVolumeVertices[25] = shadowVolume.shadowBoxes[1].nearLL.y;
  actualVolumeVertices[26] = shadowVolume.shadowBoxes[1].nearLL.z;

  actualVolumeVertices[27] = shadowVolume.shadowBoxes[1].nearLR.x;
  actualVolumeVertices[28] = shadowVolume.shadowBoxes[1].nearLR.y;
  actualVolumeVertices[29] = shadowVolume.shadowBoxes[1].nearLR.z;

  actualVolumeVertices[30] = shadowVolume.shadowBoxes[1].nearUR.x;
  actualVolumeVertices[31] = shadowVolume.shadowBoxes[1].nearUR.y;
  actualVolumeVertices[32] = shadowVolume.shadowBoxes[1].nearUR.z;

  actualVolumeVertices[33] = shadowVolume.shadowBoxes[1].nearUL.x;
  actualVolumeVertices[34] = shadowVolume.shadowBoxes[1].nearUL.y;
  actualVolumeVertices[35] = shadowVolume.shadowBoxes[1].nearUL.z;

  actualVolumeVertices[36] = shadowVolume.shadowBoxes[1].farLL.x;
  actualVolumeVertices[37] = shadowVolume.shadowBoxes[1].farLL.y;
  actualVolumeVertices[38] = shadowVolume.shadowBoxes[1].farLL.z;

  actualVolumeVertices[39] = shadowVolume.shadowBoxes[1].farLR.x;
  actualVolumeVertices[40] = shadowVolume.shadowBoxes[1].farLR.y;
  actualVolumeVertices[41] = shadowVolume.shadowBoxes[1].farLR.z;

  actualVolumeVertices[42] = shadowVolume.shadowBoxes[1].farUR.x;
  actualVolumeVertices[43] = shadowVolume.shadowBoxes[1].farUR.y;
  actualVolumeVertices[44] = shadowVolume.shadowBoxes[1].farUR.z;

  actualVolumeVertices[45] = shadowVolume.shadowBoxes[1].farUL.x;
  actualVolumeVertices[46] = shadowVolume.shadowBoxes[1].farUL.y;
  actualVolumeVertices[47] = shadowVolume.shadowBoxes[1].farUL.z;

  //far box
  actualVolumeVertices[48] = shadowVolume.shadowBoxes[2].nearLL.x;
  actualVolumeVertices[49] = shadowVolume.shadowBoxes[2].nearLL.y;
  actualVolumeVertices[50] = shadowVolume.shadowBoxes[2].nearLL.z;

  actualVolumeVertices[51] = shadowVolume.shadowBoxes[2].nearLR.x;
  actualVolumeVertices[52] = shadowVolume.shadowBoxes[2].nearLR.y;
  actualVolumeVertices[53] = shadowVolume.shadowBoxes[2].nearLR.z;

  actualVolumeVertices[54] = shadowVolume.shadowBoxes[2].nearUR.x;
  actualVolumeVertices[55] = shadowVolume.shadowBoxes[2].nearUR.y;
  actualVolumeVertices[56] = shadowVolume.shadowBoxes[2].nearUR.z;

  actualVolumeVertices[57] = shadowVolume.shadowBoxes[2].nearUL.x;
  actualVolumeVertices[58] = shadowVolume.shadowBoxes[2].nearUL.y;
  actualVolumeVertices[59] = shadowVolume.shadowBoxes[2].nearUL.z;

  actualVolumeVertices[60] = shadowVolume.shadowBoxes[2].farLL.x;
  actualVolumeVertices[61] = shadowVolume.shadowBoxes[2].farLL.y;
  actualVolumeVertices[62] = shadowVolume.shadowBoxes[2].farLL.z;

  actualVolumeVertices[63] = shadowVolume.shadowBoxes[2].farLR.x;
  actualVolumeVertices[64] = shadowVolume.shadowBoxes[2].farLR.y;
  actualVolumeVertices[65] = shadowVolume.shadowBoxes[2].farLR.z;

  actualVolumeVertices[66] = shadowVolume.shadowBoxes[2].farUR.x;
  actualVolumeVertices[67] = shadowVolume.shadowBoxes[2].farUR.y;
  actualVolumeVertices[68] = shadowVolume.shadowBoxes[2].farUR.z;

  actualVolumeVertices[69] = shadowVolume.shadowBoxes[2].farUL.x;
  actualVolumeVertices[70] = shadowVolume.shadowBoxes[2].farUL.y;
  actualVolumeVertices[71] = shadowVolume.shadowBoxes[2].farUL.z;

  //near box light source
  actualVolumeVertices[72] = shadowVolume.shadowBoxes[0].lightSource.x;
  actualVolumeVertices[73] = shadowVolume.shadowBoxes[0].lightSource.y;
  actualVolumeVertices[74] = shadowVolume.shadowBoxes[0].lightSource.z;
  //middle box light source
  actualVolumeVertices[75] = shadowVolume.shadowBoxes[1].lightSource.x;
  actualVolumeVertices[76] = shadowVolume.shadowBoxes[1].lightSource.y;
  actualVolumeVertices[77] = shadowVolume.shadowBoxes[1].lightSource.z;
  //far box light source
  actualVolumeVertices[78] = shadowVolume.shadowBoxes[2].lightSource.x;
  actualVolumeVertices[79] = shadowVolume.shadowBoxes[2].lightSource.y;
  actualVolumeVertices[80] = shadowVolume.shadowBoxes[2].lightSource.z;

  actualVolumesGLBuffers.bind(VAO | VBO | EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(actualVolumeVertices), actualVolumeVertices, GL_STATIC_DRAW);
}

void ShadowVolumeRenderer::renderActualVolume(int index)
{
  actualVolumesGLBuffers.bind(VAO | VBO | EBO);
  if (index == 0)
    glDrawElements(GL_LINE_STRIP, 24, GL_UNSIGNED_INT, 0);
  else if (index == 1)
    glDrawElementsBaseVertex(GL_LINE_STRIP, 24, GL_UNSIGNED_INT, 0, 8);
  else if (index == 2)
    glDrawElementsBaseVertex(GL_LINE_STRIP, 24, GL_UNSIGNED_INT, 0, 16);
}

void ShadowVolumeRenderer::renderLightSource(int index)
{
  glPointSize(12.0f);
  if (index == 0)
    glDrawElementsBaseVertex(GL_POINTS, 1, GL_UNSIGNED_INT, 0, 24);
  else if (index == 1)
    glDrawElementsBaseVertex(GL_POINTS, 1, GL_UNSIGNED_INT, 0, 25);
  else if (index == 2)
    glDrawElementsBaseVertex(GL_POINTS, 1, GL_UNSIGNED_INT, 0, 26);
}
