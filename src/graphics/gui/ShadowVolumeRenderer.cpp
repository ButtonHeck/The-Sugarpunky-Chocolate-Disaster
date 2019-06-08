#include "ShadowVolumeRenderer"

ShadowVolumeRenderer::ShadowVolumeRenderer(ShadowVolume &shadowVolume)
  :
    shadowVolume(shadowVolume),
    expectedVolumesGLBuffers(VAO | VBO | EBO),
    actualVolumesGLBuffers(VAO | VBO | EBO)
{
  expectedVolumesGLBuffers.bind(VAO | VBO | EBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(volumesIndices), volumesIndices, GL_STATIC_DRAW);

  actualVolumesGLBuffers.bind(VAO | VBO | EBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(volumesIndices), volumesIndices, GL_STATIC_DRAW);
}

void ShadowVolumeRenderer::bufferExpectedVolumes()
{
  //near box ground level border points
  expectedVolumesVertices[0] = shadowVolume.shadowBoxes[0].expectedLL.x;
  expectedVolumesVertices[1] = 0.001f;
  expectedVolumesVertices[2] = shadowVolume.shadowBoxes[0].expectedLL.y;

  expectedVolumesVertices[3] = shadowVolume.shadowBoxes[0].expectedLR.x;
  expectedVolumesVertices[4] = 0.001f;
  expectedVolumesVertices[5] = shadowVolume.shadowBoxes[0].expectedLR.y;

  expectedVolumesVertices[6] = shadowVolume.shadowBoxes[0].expectedUR.x;
  expectedVolumesVertices[7] = 0.001f;
  expectedVolumesVertices[8] = shadowVolume.shadowBoxes[0].expectedUR.y;

  expectedVolumesVertices[9] = shadowVolume.shadowBoxes[0].expectedUL.x;
  expectedVolumesVertices[10] = 0.001f;
  expectedVolumesVertices[11] = shadowVolume.shadowBoxes[0].expectedUL.y;

  //near box lifted border points
  expectedVolumesVertices[12] = shadowVolume.shadowBoxes[0].expectedLL.x;
  expectedVolumesVertices[13] = ShadowVolume::BOX_MIN_HEIGHT;
  expectedVolumesVertices[14] = shadowVolume.shadowBoxes[0].expectedLL.y;

  expectedVolumesVertices[15] = shadowVolume.shadowBoxes[0].expectedLR.x;
  expectedVolumesVertices[16] = ShadowVolume::BOX_MIN_HEIGHT;
  expectedVolumesVertices[17] = shadowVolume.shadowBoxes[0].expectedLR.y;

  expectedVolumesVertices[18] = shadowVolume.shadowBoxes[0].expectedUR.x;
  expectedVolumesVertices[19] = ShadowVolume::BOX_MIN_HEIGHT;
  expectedVolumesVertices[20] = shadowVolume.shadowBoxes[0].expectedUR.y;

  expectedVolumesVertices[21] = shadowVolume.shadowBoxes[0].expectedUL.x;
  expectedVolumesVertices[22] = ShadowVolume::BOX_MIN_HEIGHT;
  expectedVolumesVertices[23] = shadowVolume.shadowBoxes[0].expectedUL.y;

  //middle box ground level border points
  expectedVolumesVertices[24] = shadowVolume.shadowBoxes[1].expectedLL.x;
  expectedVolumesVertices[25] = 0.001f;
  expectedVolumesVertices[26] = shadowVolume.shadowBoxes[1].expectedLL.y;

  expectedVolumesVertices[27] = shadowVolume.shadowBoxes[1].expectedLR.x;
  expectedVolumesVertices[28] = 0.001f;
  expectedVolumesVertices[29] = shadowVolume.shadowBoxes[1].expectedLR.y;

  expectedVolumesVertices[30] = shadowVolume.shadowBoxes[1].expectedUR.x;
  expectedVolumesVertices[31] = 0.001f;
  expectedVolumesVertices[32] = shadowVolume.shadowBoxes[1].expectedUR.y;

  expectedVolumesVertices[33] = shadowVolume.shadowBoxes[1].expectedUL.x;
  expectedVolumesVertices[34] = 0.001f;
  expectedVolumesVertices[35] = shadowVolume.shadowBoxes[1].expectedUL.y;

  //middle box lifted border points
  expectedVolumesVertices[36] = shadowVolume.shadowBoxes[1].expectedLL.x;
  expectedVolumesVertices[37] = ShadowVolume::BOX_MIN_HEIGHT;
  expectedVolumesVertices[38] = shadowVolume.shadowBoxes[1].expectedLL.y;

  expectedVolumesVertices[39] = shadowVolume.shadowBoxes[1].expectedLR.x;
  expectedVolumesVertices[40] = ShadowVolume::BOX_MIN_HEIGHT;
  expectedVolumesVertices[41] = shadowVolume.shadowBoxes[1].expectedLR.y;

  expectedVolumesVertices[42] = shadowVolume.shadowBoxes[1].expectedUR.x;
  expectedVolumesVertices[43] = ShadowVolume::BOX_MIN_HEIGHT;
  expectedVolumesVertices[44] = shadowVolume.shadowBoxes[1].expectedUR.y;

  expectedVolumesVertices[45] = shadowVolume.shadowBoxes[1].expectedUL.x;
  expectedVolumesVertices[46] = ShadowVolume::BOX_MIN_HEIGHT;
  expectedVolumesVertices[47] = shadowVolume.shadowBoxes[1].expectedUL.y;

  //far box box ground level border points
  expectedVolumesVertices[48] = shadowVolume.shadowBoxes[2].expectedLL.x;
  expectedVolumesVertices[49] = 0.001f;
  expectedVolumesVertices[50] = shadowVolume.shadowBoxes[2].expectedLL.y;

  expectedVolumesVertices[51] = shadowVolume.shadowBoxes[2].expectedLR.x;
  expectedVolumesVertices[52] = 0.001f;
  expectedVolumesVertices[53] = shadowVolume.shadowBoxes[2].expectedLR.y;

  expectedVolumesVertices[54] = shadowVolume.shadowBoxes[2].expectedUR.x;
  expectedVolumesVertices[55] = 0.001f;
  expectedVolumesVertices[56] = shadowVolume.shadowBoxes[2].expectedUR.y;

  expectedVolumesVertices[57] = shadowVolume.shadowBoxes[2].expectedUL.x;
  expectedVolumesVertices[58] = 0.001f;
  expectedVolumesVertices[59] = shadowVolume.shadowBoxes[2].expectedUL.y;

  //far box lifted border points
  expectedVolumesVertices[60] = shadowVolume.shadowBoxes[2].expectedLL.x;
  expectedVolumesVertices[61] = ShadowVolume::BOX_MIN_HEIGHT;
  expectedVolumesVertices[62] = shadowVolume.shadowBoxes[2].expectedLL.y;

  expectedVolumesVertices[63] = shadowVolume.shadowBoxes[2].expectedLR.x;
  expectedVolumesVertices[64] = ShadowVolume::BOX_MIN_HEIGHT;
  expectedVolumesVertices[65] = shadowVolume.shadowBoxes[2].expectedLR.y;

  expectedVolumesVertices[66] = shadowVolume.shadowBoxes[2].expectedUR.x;
  expectedVolumesVertices[67] = ShadowVolume::BOX_MIN_HEIGHT;
  expectedVolumesVertices[68] = shadowVolume.shadowBoxes[2].expectedUR.y;

  expectedVolumesVertices[69] = shadowVolume.shadowBoxes[2].expectedUL.x;
  expectedVolumesVertices[70] = ShadowVolume::BOX_MIN_HEIGHT;
  expectedVolumesVertices[71] = shadowVolume.shadowBoxes[2].expectedUL.y;

  expectedVolumesGLBuffers.bind(VAO | VBO | EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(expectedVolumesVertices), expectedVolumesVertices, GL_STATIC_DRAW);
}

void ShadowVolumeRenderer::renderExpectedVolume(int index)
{
  expectedVolumesGLBuffers.bind(VAO | VBO | EBO);
  glDrawElementsBaseVertex(GL_LINE_STRIP, VOLUMES_INDICES_COUNT, GL_UNSIGNED_INT, 0, index * ShadowVolume::BOX_EXPECTED_VERTICES * 2);
}

void ShadowVolumeRenderer::bufferActualVolumes()
{
  //near box
  actualVolumeVertices[0] = shadowVolume.shadowBoxes[0].lsPovNearLL.x;
  actualVolumeVertices[1] = shadowVolume.shadowBoxes[0].lsPovNearLL.y;
  actualVolumeVertices[2] = shadowVolume.shadowBoxes[0].lsPovNearLL.z;

  actualVolumeVertices[3] = shadowVolume.shadowBoxes[0].lsPovNearLR.x;
  actualVolumeVertices[4] = shadowVolume.shadowBoxes[0].lsPovNearLR.y;
  actualVolumeVertices[5] = shadowVolume.shadowBoxes[0].lsPovNearLR.z;

  actualVolumeVertices[6] = shadowVolume.shadowBoxes[0].lsPovNearUR.x;
  actualVolumeVertices[7] = shadowVolume.shadowBoxes[0].lsPovNearUR.y;
  actualVolumeVertices[8] = shadowVolume.shadowBoxes[0].lsPovNearUR.z;

  actualVolumeVertices[9] = shadowVolume.shadowBoxes[0].lsPovNearUL.x;
  actualVolumeVertices[10] = shadowVolume.shadowBoxes[0].lsPovNearUL.y;
  actualVolumeVertices[11] = shadowVolume.shadowBoxes[0].lsPovNearUL.z;

  actualVolumeVertices[12] = shadowVolume.shadowBoxes[0].lsPovFarLL.x;
  actualVolumeVertices[13] = shadowVolume.shadowBoxes[0].lsPovFarLL.y;
  actualVolumeVertices[14] = shadowVolume.shadowBoxes[0].lsPovFarLL.z;

  actualVolumeVertices[15] = shadowVolume.shadowBoxes[0].lsPovFarLR.x;
  actualVolumeVertices[16] = shadowVolume.shadowBoxes[0].lsPovFarLR.y;
  actualVolumeVertices[17] = shadowVolume.shadowBoxes[0].lsPovFarLR.z;

  actualVolumeVertices[18] = shadowVolume.shadowBoxes[0].lsPovFarUR.x;
  actualVolumeVertices[19] = shadowVolume.shadowBoxes[0].lsPovFarUR.y;
  actualVolumeVertices[20] = shadowVolume.shadowBoxes[0].lsPovFarUR.z;

  actualVolumeVertices[21] = shadowVolume.shadowBoxes[0].lsPovFarUL.x;
  actualVolumeVertices[22] = shadowVolume.shadowBoxes[0].lsPovFarUL.y;
  actualVolumeVertices[23] = shadowVolume.shadowBoxes[0].lsPovFarUL.z;

  //middle box
  actualVolumeVertices[24] = shadowVolume.shadowBoxes[1].lsPovNearLL.x;
  actualVolumeVertices[25] = shadowVolume.shadowBoxes[1].lsPovNearLL.y;
  actualVolumeVertices[26] = shadowVolume.shadowBoxes[1].lsPovNearLL.z;

  actualVolumeVertices[27] = shadowVolume.shadowBoxes[1].lsPovNearLR.x;
  actualVolumeVertices[28] = shadowVolume.shadowBoxes[1].lsPovNearLR.y;
  actualVolumeVertices[29] = shadowVolume.shadowBoxes[1].lsPovNearLR.z;

  actualVolumeVertices[30] = shadowVolume.shadowBoxes[1].lsPovNearUR.x;
  actualVolumeVertices[31] = shadowVolume.shadowBoxes[1].lsPovNearUR.y;
  actualVolumeVertices[32] = shadowVolume.shadowBoxes[1].lsPovNearUR.z;

  actualVolumeVertices[33] = shadowVolume.shadowBoxes[1].lsPovNearUL.x;
  actualVolumeVertices[34] = shadowVolume.shadowBoxes[1].lsPovNearUL.y;
  actualVolumeVertices[35] = shadowVolume.shadowBoxes[1].lsPovNearUL.z;

  actualVolumeVertices[36] = shadowVolume.shadowBoxes[1].lsPovFarLL.x;
  actualVolumeVertices[37] = shadowVolume.shadowBoxes[1].lsPovFarLL.y;
  actualVolumeVertices[38] = shadowVolume.shadowBoxes[1].lsPovFarLL.z;

  actualVolumeVertices[39] = shadowVolume.shadowBoxes[1].lsPovFarLR.x;
  actualVolumeVertices[40] = shadowVolume.shadowBoxes[1].lsPovFarLR.y;
  actualVolumeVertices[41] = shadowVolume.shadowBoxes[1].lsPovFarLR.z;

  actualVolumeVertices[42] = shadowVolume.shadowBoxes[1].lsPovFarUR.x;
  actualVolumeVertices[43] = shadowVolume.shadowBoxes[1].lsPovFarUR.y;
  actualVolumeVertices[44] = shadowVolume.shadowBoxes[1].lsPovFarUR.z;

  actualVolumeVertices[45] = shadowVolume.shadowBoxes[1].lsPovFarUL.x;
  actualVolumeVertices[46] = shadowVolume.shadowBoxes[1].lsPovFarUL.y;
  actualVolumeVertices[47] = shadowVolume.shadowBoxes[1].lsPovFarUL.z;

  //far box
  actualVolumeVertices[48] = shadowVolume.shadowBoxes[2].lsPovNearLL.x;
  actualVolumeVertices[49] = shadowVolume.shadowBoxes[2].lsPovNearLL.y;
  actualVolumeVertices[50] = shadowVolume.shadowBoxes[2].lsPovNearLL.z;

  actualVolumeVertices[51] = shadowVolume.shadowBoxes[2].lsPovNearLR.x;
  actualVolumeVertices[52] = shadowVolume.shadowBoxes[2].lsPovNearLR.y;
  actualVolumeVertices[53] = shadowVolume.shadowBoxes[2].lsPovNearLR.z;

  actualVolumeVertices[54] = shadowVolume.shadowBoxes[2].lsPovNearUR.x;
  actualVolumeVertices[55] = shadowVolume.shadowBoxes[2].lsPovNearUR.y;
  actualVolumeVertices[56] = shadowVolume.shadowBoxes[2].lsPovNearUR.z;

  actualVolumeVertices[57] = shadowVolume.shadowBoxes[2].lsPovNearUL.x;
  actualVolumeVertices[58] = shadowVolume.shadowBoxes[2].lsPovNearUL.y;
  actualVolumeVertices[59] = shadowVolume.shadowBoxes[2].lsPovNearUL.z;

  actualVolumeVertices[60] = shadowVolume.shadowBoxes[2].lsPovFarLL.x;
  actualVolumeVertices[61] = shadowVolume.shadowBoxes[2].lsPovFarLL.y;
  actualVolumeVertices[62] = shadowVolume.shadowBoxes[2].lsPovFarLL.z;

  actualVolumeVertices[63] = shadowVolume.shadowBoxes[2].lsPovFarLR.x;
  actualVolumeVertices[64] = shadowVolume.shadowBoxes[2].lsPovFarLR.y;
  actualVolumeVertices[65] = shadowVolume.shadowBoxes[2].lsPovFarLR.z;

  actualVolumeVertices[66] = shadowVolume.shadowBoxes[2].lsPovFarUR.x;
  actualVolumeVertices[67] = shadowVolume.shadowBoxes[2].lsPovFarUR.y;
  actualVolumeVertices[68] = shadowVolume.shadowBoxes[2].lsPovFarUR.z;

  actualVolumeVertices[69] = shadowVolume.shadowBoxes[2].lsPovFarUL.x;
  actualVolumeVertices[70] = shadowVolume.shadowBoxes[2].lsPovFarUL.y;
  actualVolumeVertices[71] = shadowVolume.shadowBoxes[2].lsPovFarUL.z;

  //near box light source
  actualVolumeVertices[72] = shadowVolume.shadowBoxes[0].localLightSource.x;
  actualVolumeVertices[73] = shadowVolume.shadowBoxes[0].localLightSource.y;
  actualVolumeVertices[74] = shadowVolume.shadowBoxes[0].localLightSource.z;
  //middle box light source
  actualVolumeVertices[75] = shadowVolume.shadowBoxes[1].localLightSource.x;
  actualVolumeVertices[76] = shadowVolume.shadowBoxes[1].localLightSource.y;
  actualVolumeVertices[77] = shadowVolume.shadowBoxes[1].localLightSource.z;
  //far box light source
  actualVolumeVertices[78] = shadowVolume.shadowBoxes[2].localLightSource.x;
  actualVolumeVertices[79] = shadowVolume.shadowBoxes[2].localLightSource.y;
  actualVolumeVertices[80] = shadowVolume.shadowBoxes[2].localLightSource.z;

  actualVolumesGLBuffers.bind(VAO | VBO | EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(actualVolumeVertices), actualVolumeVertices, GL_STATIC_DRAW);
}

void ShadowVolumeRenderer::renderActualVolume(int index)
{
  actualVolumesGLBuffers.bind(VAO | VBO | EBO);
  glDrawElementsBaseVertex(GL_LINE_STRIP, VOLUMES_INDICES_COUNT, GL_UNSIGNED_INT, 0, index * ShadowVolume::BOX_ACTUAL_VERTICES);
}

void ShadowVolumeRenderer::renderLightSource(int index)
{
  glPointSize(LIGHT_SOURCE_POSITION_POINT_SIZE);
  glDrawElementsBaseVertex(GL_POINTS, 1, GL_UNSIGNED_INT, 0, VOLUMES_INDICES_COUNT + index);
}
