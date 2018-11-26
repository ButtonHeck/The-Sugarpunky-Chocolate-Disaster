#include "ShadowVolume.h"

ShadowVolume::ShadowVolume(TheSunFacade& sun)
  :
    sun(sun)
{}

void ShadowVolume::update(Camera& camera)
{
  glm::vec3 sunPosition = sun.getCurrentPosition();
  lightDirTo = glm::normalize(glm::vec3(0.0f) - sunPosition);
  bool isPosX = sunPosition.x > 0.0f;

  //step 1 - create key points
  const float SHADOW_NEAR_DISTANCE = 20.0f;
  const float TAN_FOV_DIV2 = glm::tan(glm::radians(FOV / 2.0f));
  glm::vec3 cameraPos = camera.getPosition();
  glm::vec3 cameraFront = camera.getDirection();
  glm::vec3 cameraRight = camera.getRight();
  glm::vec3 cameraShadowMid = cameraPos + SHADOW_NEAR_DISTANCE * cameraFront;
  glm::vec3 cameraShadowRight = cameraShadowMid + SHADOW_NEAR_DISTANCE * TAN_FOV_DIV2 * cameraRight;
  glm::vec3 cameraShadowLeft = cameraShadowMid - SHADOW_NEAR_DISTANCE * TAN_FOV_DIV2 * cameraRight;
  //step 1 - create bounding box
  float boxPosX = glm::max(glm::max(cameraShadowRight.x, cameraShadowLeft.x), cameraPos.x);
  float boxNegX = glm::min(glm::min(cameraShadowRight.x, cameraShadowLeft.x), cameraPos.x);
  float boxPosZ = glm::max(glm::max(cameraShadowRight.z, cameraShadowLeft.z), cameraPos.z);
  float boxNegZ = glm::min(glm::min(cameraShadowRight.z, cameraShadowLeft.z), cameraPos.z);
  //step 2 - correct bounding box
  float offset;
  if (lightDirTo.x < 0.0f)
    {
      offset = glm::mix(30.0f, 0.0f, sunPosition.y / 192.0f);
      boxPosX += offset;
      boxPosX = glm::min(192.0f, boxPosX);
    }
  else
    {
      offset = glm::mix(-30.0f, 0.0f, sunPosition.y / 192.0f);
      boxNegX += offset;
      boxNegX = glm::max(-192.0f, boxNegX);
    }
  //step 3 - check sunPosition
  float sunDeltaX = sunPosition.x / 192.0f;
  //step 4 - calculate box center
  float BOX_WIDTH = boxPosX - boxNegX;
  float boxCenterX = (boxPosX + boxNegX) / 2.0f;
  float boxCenterZ = (boxPosZ + boxNegZ) / 2.0f;
  //step 5 - calculate light source position
  glm::vec3 lightSource;
  lightSource.x = boxCenterX + (sunDeltaX / (float)WORLD_WIDTH) * BOX_WIDTH;
  lightSource.y = (sunPosition.y / 192.0f) * (BOX_WIDTH / 2.0f);
  lightSource.z = boxCenterZ;
  //step 6 - calculate frustum borders
  float frustumRight = glm::abs(boxNegZ - boxCenterZ);
  float frustumLeft = -frustumRight;
  float frustumTop = glm::abs(boxNegX - boxCenterX);
  float frustumBottom = -frustumTop;
  float frustumNear = 0.1f;
  float frustumFar = BOX_WIDTH;

  glm::mat4 lightProjection = glm::ortho(isPosX ? frustumRight : frustumLeft,
                                         isPosX ? frustumLeft : frustumRight,
                                         isPosX ? frustumTop : frustumBottom,
                                         isPosX ? frustumBottom : frustumTop,
                                         frustumNear,
                                         frustumFar);
  glm::mat4 lightView = glm::lookAt(lightSource, lightSource + lightDirTo, glm::vec3(0.0f, 1.0f, 0.0f));
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
