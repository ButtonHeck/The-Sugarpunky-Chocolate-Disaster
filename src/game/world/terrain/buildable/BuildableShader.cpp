#include "BuildableShader"
#include "Shader"

BuildableShader::BuildableShader(Shader &buildableRenderShader, Shader &selectedRenderShader)
  :
    buildableRenderShader(buildableRenderShader),
    selectedRenderShader(selectedRenderShader)
{}

void BuildableShader::updateBuildable(const glm::mat4 &projectionView)
{
  buildableRenderShader.use();
  buildableRenderShader.setMat4("u_projectionView", projectionView);
}

void BuildableShader::updateSelected(const glm::mat4 &projectionView, const glm::vec4 &selectedTranslation)
{
  selectedRenderShader.use();
  selectedRenderShader.setMat4("u_projectionView", projectionView);
  selectedRenderShader.setVec4("u_translation", selectedTranslation);
}
