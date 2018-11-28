#include "ShoreShader.h"

ShoreShader::ShoreShader(Shader &renderShader, Shader &normalsShader)
  :
    renderShader(renderShader),
    normalsShader(normalsShader)
{}

void ShoreShader::update(glm::vec3 &lightDir, glm::mat4 &lightSpaceMatrixNear, glm::mat4 &lightSpaceMatrixFar, glm::mat4 &projectionView, glm::vec3 &viewPosition, bool useShadows)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setBool("u_shadowEnable", useShadows);
  renderShader.setVec3("u_lightDir", -lightDir);
  renderShader.setMat4("u_lightSpaceMatrix[0]", lightSpaceMatrixNear);
  renderShader.setMat4("u_lightSpaceMatrix[1]", lightSpaceMatrixFar);
  renderShader.setVec3("u_viewPosition", viewPosition);
}

void ShoreShader::updateNormals(glm::mat4 &projectionView)
{
  normalsShader.use();
  normalsShader.setMat4("u_projectionView", projectionView);
}

void ShoreShader::debugRenderMode(bool enable)
{
  renderShader.use();
  renderShader.setBool("u_debugRenderMode", enable);
}
