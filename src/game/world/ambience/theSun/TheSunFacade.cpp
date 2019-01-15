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

void TheSunFacade::setPos(float posDegrees)
{
  model = theSun.setPos(posDegrees);
}

void TheSunFacade::draw(glm::mat4 &skyProjectionView, bool doOcclusionTest)
{
  shader.update(skyProjectionView, model);
  renderer.render(doOcclusionTest);
}

glm::vec3 TheSunFacade::getCurrentPosition() const
{
  return theSun.getPosition();
}

GLuint TheSunFacade::getSamplesPassedQueryResult()
{
  return renderer.getSamplesPassedQueryResult();
}
