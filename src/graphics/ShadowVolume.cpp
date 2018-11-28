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
  glm::vec3 camPos = camera.getPosition();
  glm::vec3 camFront = camera.getDirection();
  glm::vec3 camRight = camera.getRight();
  glm::vec3 camShadowMid = camPos + SHADOW_NEAR_DISTANCE * camFront;
  glm::vec3 camShadowRight = camShadowMid + SHADOW_NEAR_DISTANCE * TAN_FOV_DIV2 * camRight;
  glm::vec3 camShadowLeft = camShadowMid - SHADOW_NEAR_DISTANCE * TAN_FOV_DIV2 * camRight;
  glm::vec3 camFarShadowMid = camPos + SHADOW_FAR_DISTANCE * camFront;
  glm::vec3 camFarShadowRight = camFarShadowMid + SHADOW_FAR_DISTANCE * TAN_FOV_DIV2 * camRight;
  glm::vec3 camFarShadowLeft = camFarShadowMid - SHADOW_FAR_DISTANCE * TAN_FOV_DIV2 * camRight;
  //step 1 - create bounding box
  float boxPosX = glm::max(glm::max(camShadowRight.x, camShadowLeft.x), camPos.x);
  float boxNegX = glm::min(glm::min(camShadowRight.x, camShadowLeft.x), camPos.x);
  float boxPosZ = glm::max(glm::max(camShadowRight.z, camShadowLeft.z), camPos.z);
  float boxNegZ = glm::min(glm::min(camShadowRight.z, camShadowLeft.z), camPos.z);

  float boxPosXFar = glm::max(camFarShadowRight.x, glm::max(camFarShadowLeft.x, glm::max(camShadowRight.x, camShadowLeft.x)));
  float boxNegXFar = glm::min(camFarShadowRight.x, glm::min(camFarShadowLeft.x, glm::min(camShadowRight.x, camShadowLeft.x)));
  float boxPosZFar = glm::max(camFarShadowRight.z, glm::max(camFarShadowLeft.z, glm::max(camShadowRight.z, camShadowLeft.z)));
  float boxNegZFar = glm::min(camFarShadowRight.z, glm::min(camFarShadowLeft.z, glm::min(camShadowRight.z, camShadowLeft.z)));
  //step 2 - correct bounding box
  float offset;
  if (lightDirTo.x < 0.0f)
    {
      offset = glm::mix(30.0f, 0.0f, sunPosition.y / 192.0f);
      boxPosX += offset;
      boxPosX = glm::min(192.0f, boxPosX);
      boxPosXFar += offset;
      boxPosXFar = glm::min(192.0f, boxPosX);
    }
  else
    {
      offset = glm::mix(-30.0f, 0.0f, sunPosition.y / 192.0f);
      boxNegX += offset;
      boxNegX = glm::max(-192.0f, boxNegX);
      boxNegXFar += offset;
      boxNegXFar = glm::max(-192.0f, boxNegX);
    }
  //step 3 - check sunPosition
  float sunDeltaX = sunPosition.x / 192.0f;
  //step 4 - calculate box center
  float BOX_WIDTH = boxPosX - boxNegX;
  float boxCenterX = (boxPosX + boxNegX) / 2.0f;
  float boxCenterZ = (boxPosZ + boxNegZ) / 2.0f;
  float BOX_FAR_WIDTH = boxPosXFar - boxNegXFar;
  float boxFarCenterX = (boxPosXFar + boxNegXFar) / 2.0f;
  float boxFarCenterZ = (boxPosZFar + boxNegZFar) / 2.0f;
  //step 5 - calculate light source position
  glm::vec3 lightSource;
  lightSource.x = boxCenterX + (sunDeltaX / (float)WORLD_WIDTH) * BOX_WIDTH;
  lightSource.y = (sunPosition.y / 192.0f) * (BOX_WIDTH / 2.0f);
  lightSource.z = boxCenterZ;
  glm::vec3 lightSourceFar;
  lightSourceFar.x = boxFarCenterX + (sunDeltaX / (float)WORLD_WIDTH) * BOX_FAR_WIDTH;
  lightSourceFar.y = (sunPosition.y / 192.0f) * (BOX_FAR_WIDTH / 2.0f);
  lightSourceFar.z = boxFarCenterZ;
  //step 6 - calculate frustum borders
  float frustumRight = glm::abs(boxNegZ - boxCenterZ);
  float frustumLeft = -frustumRight;
  float frustumTop = glm::abs(boxNegX - boxCenterX);
  float frustumBottom = -frustumTop;
  float frustumNear = 0.1f;
  float frustumFar = BOX_WIDTH;
  float farFrustumRight = glm::abs(boxNegZFar - boxFarCenterZ);
  float farFrustumLeft = -farFrustumRight;
  float farFrustumTop = glm::abs(boxNegXFar - boxFarCenterX);
  float farFrustumBottom = -farFrustumTop;
  float farFrustumNear = 0.1f;
  float farFrustumFar = BOX_FAR_WIDTH;

  glm::mat4 lightProjectionNear = glm::ortho(isPosX ? frustumRight : frustumLeft,
                                             isPosX ? frustumLeft : frustumRight,
                                             isPosX ? frustumTop : frustumBottom,
                                             isPosX ? frustumBottom : frustumTop,
                                             frustumNear,
                                             frustumFar);
  glm::mat4 lightViewNear = glm::lookAt(lightSource, lightSource + lightDirTo, glm::vec3(0.0f, 1.0f, 0.0f));
  lightSpaceMatrixNear = lightProjectionNear * lightViewNear;

  glm::mat4 lightProjectionFar = glm::ortho(isPosX ? farFrustumRight : farFrustumLeft,
                                            isPosX ? farFrustumLeft : farFrustumRight,
                                            isPosX ? farFrustumTop : farFrustumBottom,
                                            isPosX ? farFrustumBottom : farFrustumTop,
                                            farFrustumNear,
                                            farFrustumFar);
  glm::mat4 lightViewFar = glm::lookAt(lightSourceFar, lightSourceFar + lightDirTo, glm::vec3(0.0f, 1.0f, 0.0f));
  lightSpaceMatrixFar = lightProjectionFar * lightViewFar;
}

glm::vec3 ShadowVolume::getLightDir() const
{
  return lightDirTo;
}

glm::mat4 ShadowVolume::getLightSpaceMatrixNear() const
{
  return lightSpaceMatrixNear;
}

glm::mat4 ShadowVolume::getLightSpaceMatrixFar() const
{
  return lightSpaceMatrixFar;
}
