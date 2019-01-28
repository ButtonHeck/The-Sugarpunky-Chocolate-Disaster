#include "game/world/ambience/skybox/SkyboxFacade.h"

SkyboxFacade::SkyboxFacade(Shader &renderShader)
  :
    shader(renderShader),
    skybox(),
    renderer(skybox)
{}

void SkyboxFacade::draw(const glm::mat4 &skyboxProjectionView,
                        const glm::vec3 &viewPosition,
                        const glm::vec3 &lightDir)
{
  BENCHMARK("SkyboxFacade: draw all boxes", true);
  shader.update(skyboxProjectionView, viewPosition, lightDir);
  shader.selectSkyboxType(SKYBOX_CLOUDS);
  renderer.render();
  shader.selectSkyboxType(SKYBOX_HILLS_NEAR);
  renderer.render();
  shader.selectSkyboxType(SKYBOX_HIILS_FAR);
  renderer.render();
}
