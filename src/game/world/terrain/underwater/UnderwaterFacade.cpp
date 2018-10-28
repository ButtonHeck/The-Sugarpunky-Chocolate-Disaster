#include "UnderwaterFacade.h"

UnderwaterFacade::UnderwaterFacade(Shader &renderShader)
  :
    shader(renderShader),
    generator(),
    renderer(generator)
{}

void UnderwaterFacade::draw(glm::mat4& projectionView)
{
  shader.update(projectionView);
  renderer.render();
}
