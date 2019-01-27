#include "game/world/ambience/theSun/TheSunFacade.h"

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

void TheSunFacade::draw(glm::mat4 &skyProjectionView, bool doOcclusionTest, bool useReflectionPointSize)
{
  shader.update(skyProjectionView, model);
  renderer.render(doOcclusionTest, useReflectionPointSize);
}

glm::vec3 TheSunFacade::getCurrentPosition() const
{
  return theSun.getPosition();
}

const glm::mat4 TheSunFacade::getTransform() const
{
  return model;
}

GLfloat TheSunFacade::getSunVisibilityPercentage()
{
  return renderer.getSamplesPassedQueryResult() / maxSamplesPassed;
}

void TheSunFacade::adjustSunPointSize(float pointSizeDivisor, float relfectionPointSizeDivisor)
{
  renderer.setPointSize(DEFAULT_SUN_POINT_SIZE * pointSizeDivisor);
  renderer.setReflectionPointSize(DEFAULT_SUN_POINT_SIZE * relfectionPointSizeDivisor);
  float pointSize = renderer.getPointSize();
  maxSamplesPassed = pointSize * pointSize * MULTISAMPLES;
}
