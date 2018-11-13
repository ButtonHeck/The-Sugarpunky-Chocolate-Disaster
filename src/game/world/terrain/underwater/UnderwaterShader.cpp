#include "UnderwaterShader.h"

UnderwaterShader::UnderwaterShader(Shader &renderShader)
  :
    renderShader(renderShader)
{}

void UnderwaterShader::update(glm::vec3 &lightDir, glm::mat4 &projectionView)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setVec3("u_lightDir", -lightDir);
}
