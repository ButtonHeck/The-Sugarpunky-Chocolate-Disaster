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

void BuildableFacade::drawBuildable(glm::mat4& projectionView)
{
  shader.updateBuildable(projectionView);
  {
    BENCHMARK("Renderer: draw buildable", true);
    renderer.renderBuildable();
  }
}

void BuildableFacade::drawSelected(MouseInputManager& mouseInput, glm::mat4& projectionView)
{
  if (generator.getMap()[mouseInput.getCursorMapZ()][mouseInput.getCursorMapX()] != 0)
    {
      glm::mat4 selectedModel;
      selectedModel = glm::translate(selectedModel, glm::vec3(-HALF_WORLD_WIDTH + mouseInput.getCursorMapX(), 0.0f, -HALF_WORLD_HEIGHT + mouseInput.getCursorMapZ()));
      shader.updateSelected(projectionView, selectedModel);
      renderer.renderSelected();
    }
}

const map2D_f &BuildableFacade::getMap() const
{
  return generator.getMap();
}
