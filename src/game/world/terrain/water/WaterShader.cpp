#include "WaterShader"
#include "Frustum"
#include "Shader"

#include <GLFW/glfw3.h>

WaterShader::WaterShader(Shader &renderShader, Shader &cullingShader, Shader &normalsShader)
  :
    renderShader(renderShader),
    cullingShader(cullingShader),
    normalsShader(normalsShader)
{}

void WaterShader::setupCulling()
{
  const unsigned int TRANSFORM_FEEDBACK_OUTPUT_ATTRIBUTES_COUNT = 1;
  const GLchar* varyings[TRANSFORM_FEEDBACK_OUTPUT_ATTRIBUTES_COUNT] = {"o_pos"};
  glTransformFeedbackVaryings(cullingShader.getID(), TRANSFORM_FEEDBACK_OUTPUT_ATTRIBUTES_COUNT, varyings, GL_INTERLEAVED_ATTRIBS);
  cullingShader.link();
}

void WaterShader::update(const glm::vec3 &lightDir,
                         const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                         const glm::mat4 &projectionView,
                         const glm::vec3 &viewPosition,
                         const Frustum &viewFrustum,
                         bool useFrustumCulling)
{
  static float dudvMoveOffset = 0.0f;
  if (useFrustumCulling)
    {
      cullingShader.use();
      cullingShader.setVec4("u_frustumPlanes[0]", viewFrustum.getPlane(FRUSTUM_LEFT));
      cullingShader.setVec4("u_frustumPlanes[1]", viewFrustum.getPlane(FRUSTUM_RIGHT));
      cullingShader.setVec4("u_frustumPlanes[2]", viewFrustum.getPlane(FRUSTUM_BOTTOM));
      cullingShader.setVec4("u_frustumPlanes[3]", viewFrustum.getPlane(FRUSTUM_TOP));
      cullingShader.setVec4("u_frustumPlanes[4]", viewFrustum.getPlane(FRUSTUM_BACK));
    }
  renderShader.use();
  renderShader.setFloat("u_time", glfwGetTime());
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setVec3("u_viewPosition", viewPosition);
  renderShader.setVec3("u_lightDir", -lightDir);
  renderShader.setMat4("u_lightSpaceMatrix[0]", lightSpaceMatrices[0]);
  renderShader.setMat4("u_lightSpaceMatrix[1]", lightSpaceMatrices[1]);
  renderShader.setMat4("u_lightSpaceMatrix[2]", lightSpaceMatrices[2]);
  renderShader.setFloat("u_dudvMoveOffset", dudvMoveOffset);

  dudvMoveOffset += DUDV_SPEED;
  if (dudvMoveOffset >= 1.0f)
    dudvMoveOffset = 0.0f;
}

void WaterShader::updateNormals(const glm::mat4 &projectionView)
{
  normalsShader.use();
  normalsShader.setMat4("u_projectionView", projectionView);
}

void WaterShader::debugRenderMode(bool enable)
{
  renderShader.use();
  renderShader.setBool("u_debugRenderMode", enable);
}
