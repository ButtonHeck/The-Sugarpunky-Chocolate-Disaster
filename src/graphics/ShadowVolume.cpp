#include "ShadowVolume.h"

ShadowVolume::ShadowVolume(TheSunFacade& sun)
  :
    sun(sun)
{}

void ShadowVolume::update()
{
  glm::vec3 currentPosition = sun.getCurrentPosition();
  lightDirTo = glm::normalize(glm::vec3(0.0f) - currentPosition);
  glm::mat4 lightProjection = glm::ortho(LEFT_PLANE, RIGHT_PLANE, bottomPlane, topPlane, nearPlane, farPlane);
  glm::mat4 lightView = glm::lookAt(currentPosition, currentPosition + lightDirTo, glm::vec3(0.0f, 1.0f, 0.0f));
  lightSpaceMatrix = lightProjection * lightView;
}

glm::vec3 ShadowVolume::getLightDir() const
{
  return lightDirTo;
}

glm::mat4 ShadowVolume::getLightSpaceMatrix() const
{
  return lightSpaceMatrix;
}
