#include "LandShader.h"

LandShader::LandShader(Shader &renderShader)
  :
    renderShader(renderShader)
{}

void LandShader::update(glm::vec3 &lightDir, glm::mat4 &lightSpaceMatrixNear, glm::mat4 &lightSpaceMatrixFar, glm::mat4 &projectionView, glm::vec3 &viewPosition, bool useShadows)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setBool("u_shadowEnable", useShadows);
  renderShader.setVec3("u_lightDir", -lightDir);
  renderShader.setMat4("u_lightSpaceMatrix[0]", lightSpaceMatrixNear);
  renderShader.setMat4("u_lightSpaceMatrix[1]", lightSpaceMatrixFar);
  renderShader.setVec3("u_viewPosition", viewPosition);
}
