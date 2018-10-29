#include "WaterShader.h"

WaterShader::WaterShader(Shader &renderShader, Shader &cullingShader)
  :
    renderShader(renderShader),
    cullingShader(cullingShader)
{}

void WaterShader::setupCulling()
{
  const GLchar* varyings[2] = {"o_pos", "o_normal"};
  glTransformFeedbackVaryings(cullingShader.getID(), 2, varyings, GL_INTERLEAVED_ATTRIBS);
  cullingShader.link();
}

void WaterShader::update(bool useFC, glm::mat4 &projectionView, glm::vec3 &viewPosition, Frustum &viewFrustum)
{
  if (useFC)
    {
      cullingShader.use();
      cullingShader.setVec4("u_frustumPlanes[0]", viewFrustum.getPlane(FRUSTUM_LEFT));
      cullingShader.setVec4("u_frustumPlanes[1]", viewFrustum.getPlane(FRUSTUM_RIGHT));
      cullingShader.setVec4("u_frustumPlanes[2]", viewFrustum.getPlane(FRUSTUM_BOTTOM));
      cullingShader.setVec4("u_frustumPlanes[3]", viewFrustum.getPlane(FRUSTUM_TOP));
      cullingShader.setVec4("u_frustumPlanes[4]", viewFrustum.getPlane(FRUSTUM_BACK));
    }
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setVec3("u_viewPosition", viewPosition);
}
