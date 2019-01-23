#include "game/world/terrain/hills/HillsShader.h"

HillsShader::HillsShader(Shader &renderShader, Shader &cullingShader, Shader &normalsShader)
  :
    renderShader(renderShader),
    cullingShader(cullingShader),
    normalsShader(normalsShader)
{}

void HillsShader::setupCulling()
{
  const GLchar* varyings[3] = {"o_pos", "o_texCoords", "o_normal"};
  glTransformFeedbackVaryings(cullingShader.getID(), 3, varyings, GL_INTERLEAVED_ATTRIBS);
  cullingShader.link();
}

void HillsShader::update(glm::vec3 &lightDir,
                         const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                         glm::mat4 &projectionView,
                         glm::vec3 &viewPosition,
                         Frustum &viewFrustum,
                         float maxHillHeight,
                         bool useFC,
                         bool useShadows)
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
  renderShader.setVec3("u_lightDir", -lightDir);
  renderShader.setMat4("u_lightSpaceMatrix[0]", lightSpaceMatrices[0]);
  renderShader.setMat4("u_lightSpaceMatrix[1]", lightSpaceMatrices[1]);
  renderShader.setMat4("u_lightSpaceMatrix[2]", lightSpaceMatrices[2]);
}

void HillsShader::updateNormals(glm::mat4 &projectionView)
{
  normalsShader.use();
  normalsShader.setMat4("u_projectionView", projectionView);
}

void HillsShader::debugRenderMode(bool enable)
{
  renderShader.use();
  renderShader.setBool("u_debugRenderMode", enable);
}
