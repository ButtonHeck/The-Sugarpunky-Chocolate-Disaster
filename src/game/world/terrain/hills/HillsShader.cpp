#include "HillsShader.h"

HillsShader::HillsShader(Shader &renderShader, Shader &cullingShader)
  :
    renderShader(renderShader),
    cullingShader(cullingShader)
{}

void HillsShader::setupCulling()
{
  const GLchar* varyings[3] = {"o_pos", "o_texCoords", "o_normal"};
  glTransformFeedbackVaryings(cullingShader.getID(), 3, varyings, GL_INTERLEAVED_ATTRIBS);
  cullingShader.link();
}

void HillsShader::update(bool useFC,
                         bool useShadows,
                         glm::mat4 &projectionView,
                         glm::vec3 &viewPosition,
                         Frustum &viewFrustum,
                         float maxHillHeight)
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
  renderShader.setBool("u_shadowEnable", useShadows);
  renderShader.setFloat("u_maxHillHeight", maxHillHeight);
}
