#ifndef BUILDABLEFACADE_H
#define BUILDABLEFACADE_H
#include <memory>
#include "game/world/terrain/buildable/BuildableGenerator.h"
#include "game/world/terrain/buildable/BuildableShader.h"
#include "game/world/terrain/buildable/BuildableRenderer.h"
#include "util/BenchmarkTimer.h"
#include "input/MouseInputManager.h"

class BuildableFacade
{
public:
  BuildableFacade(Shader& buildableRenderShader, Shader& selectedRenderShader);
  void setup(std::vector<std::vector<float>>& landMap, std::vector<std::vector<float>>& hillsMap);
  void drawBuildable(glm::mat4 &projectionView);
  void drawSelected(MouseInputManager& mouseInput, glm::mat4& projectionView);
  std::vector<std::vector<float>>& getMap() const;

private:
  BuildableShader shader;
  std::shared_ptr<BuildableGenerator> generator;
  BuildableRenderer renderer;
};

#endif // BUILDABLEFACADE_H
