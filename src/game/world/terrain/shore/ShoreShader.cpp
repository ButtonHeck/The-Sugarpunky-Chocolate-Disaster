#include "ShoreShader.h"

ShoreShader::ShoreShader(Shader &renderShader, Shader &normalsShader)
  :
    renderShader(renderShader),
    normalsShader(normalsShader)
{}

void ShoreShader::update(glm::vec3 &lightDir, glm::mat4 &lightSpaceMatrix, glm::mat4 &projectionView, bool useShadows)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setBool("u_shadowEnable", useShadows);
  renderShader.setVec3("u_lightDir", -lightDir);
  renderShader.setMat4("u_lightSpaceMatrix", lightSpaceMatrix);
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
