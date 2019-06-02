#include "BuildableFacade.h"

BuildableFacade::BuildableFacade(Shader &buildableRenderShader, Shader &selectedRenderShader)
  :
    shader(buildableRenderShader, selectedRenderShader),
    generator(),
    renderer(generator)
{}

void BuildableFacade::setup(const map2D_f &landMap, const map2D_f &hillsMap)
{
  generator.setup(landMap, hillsMap);
}

void BuildableFacade::drawBuildable(const glm::mat4& projectionView)
{
  shader.updateBuildable(projectionView);
  {
    BENCHMARK("Buildable: draw buildable", true);
    renderer.renderBuildable();
  }
}

void BuildableFacade::drawSelected(const glm::mat4& projectionView, MouseInputManager& mouseInput)
{
  if (generator.getMap()[mouseInput.getCursorWorldZ()][mouseInput.getCursorWorldX()] != 0)
    {
      BENCHMARK("Buildable: draw selected", true);
      glm::vec4 translationVector(-HALF_WORLD_WIDTH + mouseInput.getCursorWorldX(), 0.01f, -HALF_WORLD_HEIGHT + mouseInput.getCursorWorldZ(), 0.0f);
      shader.updateSelected(projectionView, translationVector);
      renderer.renderSelected();
    }
}

const map2D_f &BuildableFacade::getMap() const
{
  return generator.getMap();
}
