#include "UnderwaterFacade.h"

UnderwaterFacade::UnderwaterFacade(Shader &renderShader)
  :
    shader(renderShader),
    generator(),
    renderer(generator)
{}

void UnderwaterFacade::draw(glm::mat4& projectionView)
{
  BENCHMARK("UnderwaterRenderer: draw", true);
  shader.update(projectionView);
  renderer.render();
}
