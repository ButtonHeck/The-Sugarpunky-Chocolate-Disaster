#pragma once

#include "BuildableGenerator"
#include "BuildableShader"
#include "BuildableRenderer"
#include "BenchmarkTimer"
#include "MouseInputManager"

class BuildableFacade
{
public:
  BuildableFacade(Shader& buildableRenderShader, Shader& selectedRenderShader);
  void setup(const map2D_f& landMap, const map2D_f& hillsMap);
  void drawBuildable(const glm::mat4 &projectionView);
  void drawSelected(const glm::mat4 &projectionView, MouseInputManager& mouseInput);
  const map2D_f& getMap() const;

private:
  BuildableShader shader;
  BuildableGenerator generator;
  BuildableRenderer renderer;
};
