#include "game/world/ambience/theSun/TheSunFacade.h"

TheSunFacade::TheSunFacade(Shader &renderShader)
  :
    shader(renderShader),
    theSun(),
    renderer(theSun)
{}

extern float debug_sunSpeed;

void TheSunFacade::move(float angleRadians)
{
  rotationTransform = theSun.move(angleRadians * debug_sunSpeed);
}

void TheSunFacade::moveAbsolutePosition(float angleRadians)
{
  rotationTransform = theSun.moveAbsolutePosition(angleRadians);
}

void TheSunFacade::draw(glm::mat4 &skyProjectionView, bool doOcclusionTest, bool useReflectionPointSize)
{
  shader.update(skyProjectionView, rotationTransform);
  renderer.render(doOcclusionTest, useReflectionPointSize);
}

glm::vec3 TheSunFacade::getCurrentPosition() const
{
  return theSun.getPosition();
}

const glm::mat4 TheSunFacade::getRotationTransform() const
{
  return rotationTransform;
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
