#include "TheSunFacade.h"

TheSunFacade::TheSunFacade(Shader &renderShader)
  :
    shader(renderShader),
    theSun(),
    renderer(theSun)
{}

extern float debug_sunSpeed;

void TheSunFacade::move(float delta)
{
  model = theSun.move(delta * debug_sunSpeed);
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
