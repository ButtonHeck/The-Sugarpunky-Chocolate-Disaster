#include "UnderwaterShader.h"

UnderwaterShader::UnderwaterShader(Shader &renderShader)
  :
    renderShader(renderShader)
{}

void UnderwaterShader::update(const glm::vec3 &lightDir, const glm::mat4 &projectionView)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setVec3("u_lightDir", -lightDir);
}
