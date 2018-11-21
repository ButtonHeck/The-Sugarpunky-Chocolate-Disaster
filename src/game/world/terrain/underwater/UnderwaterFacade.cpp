#include "UnderwaterFacade.h"

UnderwaterFacade::UnderwaterFacade(Shader &renderShader)
  :
    shader(renderShader),
    generator(),
    renderer(generator)
{}

void UnderwaterFacade::draw(glm::vec3 &lightDir, glm::mat4& projectionView)
{
  BENCHMARK("UnderwaterRenderer: draw", true);
  shader.update(lightDir, projectionView);
  renderer.render();
}
