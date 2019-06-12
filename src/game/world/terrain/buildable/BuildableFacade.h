#pragma once

#include "BuildableGenerator"
#include "BuildableShader"
#include "BuildableRenderer"

class MouseInputManager;

class BuildableFacade
{
public:
  BuildableFacade(Shader& buildableRenderShader, Shader& selectedRenderShader) noexcept;
  void setup(const map2D_f& landMap, const map2D_f& hillsMap);
  void drawBuildable(const glm::mat4 &projectionView);
  void drawSelected(const glm::mat4 &projectionView, MouseInputManager& mouseInput);
  const map2D_f& getMap() const noexcept;

private:
  BuildableShader shader;
  BuildableGenerator generator;
  BuildableRenderer renderer;
};
