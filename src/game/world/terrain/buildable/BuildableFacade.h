#ifndef BUILDABLEFACADE_H
#define BUILDABLEFACADE_H
#include "game/world/terrain/buildable/BuildableGenerator.h"
#include "game/world/terrain/buildable/BuildableShader.h"
#include "game/world/terrain/buildable/BuildableRenderer.h"
#include "util/BenchmarkTimer.h"
#include "input/MouseInputManager.h"

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
