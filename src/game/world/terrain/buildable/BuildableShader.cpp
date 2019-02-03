#include "game/world/terrain/buildable/BuildableShader.h"

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

void BuildableShader::updateSelected(const glm::mat4 &projectionView, const glm::mat4 &selectedModel)
{
  selectedRenderShader.use();
  selectedRenderShader.setMat4("u_projectionView", projectionView);
  selectedRenderShader.setMat4("u_model", selectedModel);
}
