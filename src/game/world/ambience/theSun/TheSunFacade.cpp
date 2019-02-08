#include "game/world/ambience/theSun/TheSunFacade.h"

TheSunFacade::TheSunFacade(Shader &renderShader, const ScreenResolution &screenResolution)
  :
    theSun(),
    shader(renderShader),
    renderer(theSun)
{
  float pointSizeDivisorX = screenResolution.getWidthRatioToReference();
  float pointSizeDivisorY = screenResolution.getHeightRatioToReference();
  float pointSizeDivisor = (pointSizeDivisorX + pointSizeDivisorY) / 2;
  float reflectionPointSizeDivisorX = FRAME_WATER_REFLECTION_WIDTH / ScreenResolution::REFERENCE_WIDTH;
  float reflectionPointSizeDivisorY = FRAME_WATER_REFLECTION_HEIGHT / ScreenResolution::REFERENCE_HEIGHT;
  float reflectionPointSizeDivisor = (reflectionPointSizeDivisorX + reflectionPointSizeDivisorY) / 2;

  renderer.setPointSize(DEFAULT_SUN_POINT_SIZE * pointSizeDivisor);
  renderer.setReflectionPointSize(DEFAULT_SUN_POINT_SIZE * reflectionPointSizeDivisor);
  float pointSize = renderer.getPointSize();
  maxSamplesPassed = pointSize * pointSize;
  maxSamplesPassedMultisampling = maxSamplesPassed * MULTISAMPLES;
}

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

const glm::vec3 &TheSunFacade::getLightDir() const
{
  return theSun.getLightDir();
}

const glm::mat4& TheSunFacade::getRotationTransform() const
{
  return theSun.getRotationTransform();
}

GLfloat TheSunFacade::getSunVisibility(bool multisampled) const
{
  return renderer.getSamplesPassedQueryResult() / (multisampled ? maxSamplesPassedMultisampling : maxSamplesPassed);
}
