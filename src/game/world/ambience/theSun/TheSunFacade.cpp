#include "game/world/ambience/theSun/TheSunFacade.h"

TheSunFacade::TheSunFacade(Shader &renderShader)
  :
    theSun(),
    shader(renderShader),
    renderer(theSun)
{}

extern float debug_sunSpeed;

void TheSunFacade::move(float angleRadians)
{
  theSun.move(angleRadians * debug_sunSpeed);
}

void TheSunFacade::moveAbsolutePosition(float angleRadians)
{
  theSun.moveAbsolutePosition(angleRadians);
}

void TheSunFacade::draw(const glm::mat4 &skyProjectionView,
                        bool doOcclusionTest,
                        bool useReflectionPointSize)
{
  shader.update(skyProjectionView, theSun.getRotationTransform());
  renderer.render(doOcclusionTest, useReflectionPointSize);
}

const glm::vec3& TheSunFacade::getPosition() const
{
  return theSun.getPosition();
}

const glm::mat4& TheSunFacade::getRotationTransform() const
{
  return theSun.getRotationTransform();
}

GLfloat TheSunFacade::getSunVisibilityPercentage() const
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
