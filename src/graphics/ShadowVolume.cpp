#include "ShadowVolume.h"

ShadowVolume::ShadowVolume(TheSunFacade& sun)
  :
    sun(sun)
{}

void ShadowVolume::update()
{
  glm::vec3 currentPosition = sun.getCurrentPosition();
  lightDirTo = glm::normalize(glm::vec3(0.0f) - currentPosition);
  bool isPosX = currentPosition.x > 0.0f;
  glm::mat4 lightProjection = glm::ortho(isPosX ? RIGHT_PLANE : LEFT_PLANE,
                                         isPosX ? LEFT_PLANE : RIGHT_PLANE,
                                         isPosX ? topPlane : bottomPlane,
                                         isPosX ? bottomPlane : topPlane,
                                         nearPlane, farPlane);
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
