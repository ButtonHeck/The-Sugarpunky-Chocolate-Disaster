#include "TheSunFacade.h"

TheSunFacade::TheSunFacade(Shader &renderShader)
  :
    shader(renderShader),
    theSun(),
    renderer(theSun)
{}

void TheSunFacade::move()
{
  model = theSun.move(glfwGetTime() * 0.5f);
}

void TheSunFacade::draw(glm::mat4 &skyProjectionView)
{
  shader.update(skyProjectionView, model);
  renderer.render();
}

glm::vec3 TheSunFacade::getCurrentPosition() const
{
  return theSun.getPosition();
}
