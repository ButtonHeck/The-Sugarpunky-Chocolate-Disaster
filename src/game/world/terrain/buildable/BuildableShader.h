#pragma once

#include "Shader"

class BuildableShader
{
public:
  BuildableShader(Shader& buildableRenderShader, Shader& selectedRenderShader);
  void updateBuildable(const glm::mat4 &projectionView);
  void updateSelected(const glm::mat4 &projectionView, const glm::vec4 &selectedTranslation);

private:
  Shader& buildableRenderShader;
  Shader& selectedRenderShader;
};
