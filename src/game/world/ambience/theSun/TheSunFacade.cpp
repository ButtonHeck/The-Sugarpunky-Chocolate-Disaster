#include "TheSunFacade.h"

TheSunFacade::TheSunFacade(Shader &renderShader)
  :
    shader(renderShader),
    theSun(),
    renderer(theSun)
{}

void TheSunFacade::draw(glm::mat4 &skyProjectionView)
{
  glm::mat4 model = theSun.move(glfwGetTime());
  shader.update(skyProjectionView, model);
  renderer.render();
}
