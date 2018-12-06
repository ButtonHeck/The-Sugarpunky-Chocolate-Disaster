#include "LandShader.h"

LandShader::LandShader(Shader &renderShader)
  :
    renderShader(renderShader)
{}

void LandShader::update(glm::vec3 &lightDir, const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices, glm::mat4 &projectionView, bool useShadows)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setBool("u_shadowEnable", useShadows);
  renderShader.setVec3("u_lightDir", -lightDir);
  renderShader.setMat4("u_lightSpaceMatrix[0]", lightSpaceMatrices[0]);
  renderShader.setMat4("u_lightSpaceMatrix[1]", lightSpaceMatrices[1]);
  renderShader.setMat4("u_lightSpaceMatrix[2]", lightSpaceMatrices[2]);
}
