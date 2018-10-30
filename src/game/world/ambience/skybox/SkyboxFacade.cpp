#include "SkyboxFacade.h"

SkyboxFacade::SkyboxFacade(Shader &renderShader)
  :
    shader(renderShader),
    skybox(),
    renderer(skybox)
{}

void SkyboxFacade::draw(glm::mat4 &skyProjectionView, glm::vec3 &viewPosition)
{
  shader.update(skyProjectionView, viewPosition, 2, true);
  renderer.render();
  shader.update(skyProjectionView, viewPosition, 0, false);
  renderer.render();
  shader.update(skyProjectionView, viewPosition, 1, false);
  renderer.render();
}
