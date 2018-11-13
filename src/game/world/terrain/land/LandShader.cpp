#include "LandShader.h"

LandShader::LandShader(Shader &renderShader)
  :
    renderShader(renderShader)
{}

void LandShader::update(glm::vec3 &lightDir, glm::mat4 &lightSpaceMatrix, glm::mat4 &projectionView, bool useShadows)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setBool("u_shadowEnable", useShadows);
  renderShader.setVec3("u_lightDir", -lightDir);
  renderShader.setMat4("u_lightSpaceMatrix", lightSpaceMatrix);
}
