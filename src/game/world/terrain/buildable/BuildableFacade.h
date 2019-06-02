#ifndef BUILDABLEFACADE_H
#define BUILDABLEFACADE_H
#include "BuildableGenerator.h"
#include "BuildableShader.h"
#include "BuildableRenderer.h"
#include "../../src/util/BenchmarkTimer.h"
#include "../../src/input/MouseInputManager.h"

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

#endif // BUILDABLEFACADE_H
