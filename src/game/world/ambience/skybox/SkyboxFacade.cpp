#include "game/world/ambience/skybox/SkyboxFacade.h"

SkyboxFacade::SkyboxFacade(Shader &renderShader)
  :
    shader(renderShader),
    skybox(),
    renderer(skybox)
{}

void SkyboxFacade::draw(glm::mat4 &skyProjectionView, glm::vec3 &viewPosition, glm::vec3 &lightDir)
{
  BENCHMARK("SkyboxFacade: draw all boxes", true);
  shader.update(skyProjectionView, viewPosition, 2, true, lightDir);
  renderer.render();
  shader.update(skyProjectionView, viewPosition, 0, false, lightDir);
  renderer.render();
  shader.update(skyProjectionView, viewPosition, 1, false, lightDir);
  renderer.render();
}
