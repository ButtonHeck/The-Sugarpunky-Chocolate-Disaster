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
  void drawBuildable(glm::mat4 &projectionView);
  void drawSelected(MouseInputManager& mouseInput, glm::mat4& projectionView);
  const map2D_f& getMap() const;

private:
  BuildableShader shader;
  BuildableGenerator generator;
  BuildableRenderer renderer;
};

#endif // BUILDABLEFACADE_H
