#pragma once

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class Shader;

class BuildableShader
{
public:
  BuildableShader(Shader& buildableRenderShader, Shader& selectedRenderShader) noexcept;
  void updateBuildable(const glm::mat4 &projectionView);
  void updateSelected(const glm::mat4 &projectionView, const glm::vec4 &selectedTranslation);

private:
  Shader& buildableRenderShader;
  Shader& selectedRenderShader;
};
