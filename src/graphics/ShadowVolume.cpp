#include "ShadowVolume.h"

ShadowVolume::ShadowVolume(TheSunFacade& sun)
  :
    sun(sun)
{}

void ShadowVolume::update(Camera& camera, Frustum& nearFrustum, Frustum &farFrustum, float aspect)
{
  glm::vec3 sunPosition = sun.getCurrentPosition();
  lightDirTo = glm::normalize(glm::vec3(0.0f) - sunPosition);
  bool isPosX = sunPosition.x > 0.0f;

  float boxPosX_n = std::max({nearFrustum.nearLL.x,
                              nearFrustum.nearLR.x,
                              nearFrustum.nearUR.x,
                              nearFrustum.nearUL.x,
                              nearFrustum.farLL.x,
                              nearFrustum.farLR.x,
                              nearFrustum.farUR.x,
                              nearFrustum.farUL.x,});
  float boxNegX_n = std::min({nearFrustum.nearLL.x,
                              nearFrustum.nearLR.x,
                              nearFrustum.nearUR.x,
                              nearFrustum.nearUL.x,
                              nearFrustum.farLL.x,
                              nearFrustum.farLR.x,
                              nearFrustum.farUR.x,
                              nearFrustum.farUL.x,});
  float boxPosZ_n = std::max({nearFrustum.nearLL.z,
                              nearFrustum.nearLR.z,
                              nearFrustum.nearUR.z,
                              nearFrustum.nearUL.z,
                              nearFrustum.farLL.z,
                              nearFrustum.farLR.z,
                              nearFrustum.farUR.z,
                              nearFrustum.farUL.z,});
  float boxNegZ_n = std::min({nearFrustum.nearLL.z,
                              nearFrustum.nearLR.z,
                              nearFrustum.nearUR.z,
                              nearFrustum.nearUL.z,
                              nearFrustum.farLL.z,
                              nearFrustum.farLR.z,
                              nearFrustum.farUR.z,
                              nearFrustum.farUL.z,});

  float boxPosX_f = std::max({farFrustum.nearLL.x,
                              farFrustum.nearLR.x,
                              farFrustum.nearUR.x,
                              farFrustum.nearUL.x,
                              farFrustum.farLL.x,
                              farFrustum.farLR.x,
                              farFrustum.farUR.x,
                              farFrustum.farUL.x,});
  float boxNegX_f = std::min({farFrustum.nearLL.x,
                              farFrustum.nearLR.x,
                              farFrustum.nearUR.x,
                              farFrustum.nearUL.x,
                              farFrustum.farLL.x,
                              farFrustum.farLR.x,
                              farFrustum.farUR.x,
                              farFrustum.farUL.x,});
  float boxPosZ_f = std::max({farFrustum.nearLL.z,
                              farFrustum.nearLR.z,
                              farFrustum.nearUR.z,
                              farFrustum.nearUL.z,
                              farFrustum.farLL.z,
                              farFrustum.farLR.z,
                              farFrustum.farUR.z,
                              farFrustum.farUL.z,});
  float boxNegZ_f = std::min({farFrustum.nearLL.z,
                              farFrustum.nearLR.z,
                              farFrustum.nearUR.z,
                              farFrustum.nearUL.z,
                              farFrustum.farLL.z,
                              farFrustum.farLR.z,
                              farFrustum.farUR.z,
                              farFrustum.farUL.z,});


//  const float VFOV = FOV / aspect;
//  const float TAN_VFOV_DIV2 = glm::tan(glm::radians(VFOV / 2.0f));

  //step 1 - create key points
//  glm::vec3 camPos = camera.getPosition();
//  glm::vec3 camFront = camera.getDirection();
//  glm::vec3 camRight = camera.getRight();
//  glm::vec3 camUp = camera.getUp();

//  glm::vec3 camMid_n = camPos + SHADOW_NEAR_DISTANCE * camFront;
//  glm::vec3 camRight_n = camMid_n + SHADOW_NEAR_DISTANCE * TAN_FOV_DIV2 * camRight;
//  glm::vec3 camLeft_n = camMid_n - SHADOW_NEAR_DISTANCE * TAN_FOV_DIV2 * camRight;
//  glm::vec3 camLL_n = camLeft_n - SHADOW_NEAR_DISTANCE * TAN_VFOV_DIV2 * camUp;
//  glm::vec3 camUL_n = camLeft_n + SHADOW_NEAR_DISTANCE * TAN_VFOV_DIV2 * camUp;
//  glm::vec3 camLR_n = camRight_n - SHADOW_NEAR_DISTANCE * TAN_VFOV_DIV2 * camUp;
//  glm::vec3 camUR_n = camRight_n + SHADOW_NEAR_DISTANCE * TAN_VFOV_DIV2 * camUp;

//  glm::vec3 camMid_f = camPos + SHADOW_FAR_DISTANCE * camFront;
//  glm::vec3 camRight_f = camMid_f + SHADOW_FAR_DISTANCE * TAN_FOV_DIV2 * camRight;
//  glm::vec3 camLeft_f = camMid_f - SHADOW_FAR_DISTANCE * TAN_FOV_DIV2 * camRight;
//  glm::vec3 camLL_f = camLeft_f - SHADOW_FAR_DISTANCE * TAN_VFOV_DIV2 * camUp;
//  glm::vec3 camUL_f = camLeft_f + SHADOW_FAR_DISTANCE * TAN_VFOV_DIV2 * camUp;
//  glm::vec3 camLR_f = camRight_f - SHADOW_FAR_DISTANCE * TAN_VFOV_DIV2 * camUp;
//  glm::vec3 camUR_f = camRight_f + SHADOW_FAR_DISTANCE * TAN_VFOV_DIV2 * camUp;

  //step 1 - create bounding box
//  float boxPosX_n = std::max({camLL_n.x, camUL_n.x, camLR_n.x, camUR_n.x, camPos.x});
//  float boxNegX_n = std::min({camLL_n.x, camUL_n.x, camLR_n.x, camUR_n.x, camPos.x});
//  float boxPosZ_n = std::max({camLL_n.z, camUL_n.z, camLR_n.z, camUR_n.z, camPos.z});
//  float boxNegZ_n = std::min({camLL_n.z, camUL_n.z, camLR_n.z, camUR_n.z, camPos.z});
//  float boxPosX_f = std::max({camLL_n.x, camUL_n.x, camLR_n.x, camUR_n.x, camLL_f.x, camUL_f.x, camLR_f.x, camUR_f.x});
//  float boxNegX_f = std::min({camLL_n.x, camUL_n.x, camLR_n.x, camUR_n.x, camLL_f.x, camUL_f.x, camLR_f.x, camUR_f.x});
//  float boxPosZ_f = std::max({camLL_n.z, camUL_n.z, camLR_n.z, camUR_n.z, camLL_f.z, camUL_f.z, camLR_f.z, camUR_f.z});
//  float boxNegZ_f = std::min({camLL_n.z, camUL_n.z, camLR_n.z, camUR_n.z, camLL_f.z, camUL_f.z, camLR_f.z, camUR_f.z});

  //step 2 - correct bounding box
  float offset;
  if (lightDirTo.x < 0.0f)
    {
      offset = glm::mix(30.0f, 0.0f, sunPosition.y / 192.0f);
      boxPosX_n += offset;
      boxPosX_n = glm::min(192.0f, boxPosX_n);
      boxPosX_f += offset;
      boxPosX_f = glm::min(192.0f, boxPosX_f);
    }
  else
    {
      offset = glm::mix(-30.0f, 0.0f, sunPosition.y / 192.0f);
      boxNegX_n += offset;
      boxNegX_n = glm::max(-192.0f, boxNegX_n);
      boxNegX_f += offset;
      boxNegX_f = glm::max(-192.0f, boxNegX_f);
    }
  //step 3 - check sunPosition
  float sunDeltaX = sunPosition.x / 192.0f;
  //step 4 - calculate box center
  float BOX_WIDTH_N = boxPosX_n - boxNegX_n;
  float boxMidX_n = (boxPosX_n + boxNegX_n) / 2.0f;
  float boxMidZ_n = (boxPosZ_n + boxNegZ_n) / 2.0f;
  float BOX_WIDTH_F = boxPosX_f - boxNegX_f;
  float boxMidX_f = (boxPosX_f + boxNegX_f) / 2.0f;
  float boxMidZ_f = (boxPosZ_f + boxNegZ_f) / 2.0f;
  //step 5 - calculate light source position
  glm::vec3 lightSource_n;
  lightSource_n.x = boxMidX_n + (sunDeltaX / (float)WORLD_WIDTH) * BOX_WIDTH_N;
  lightSource_n.y = (sunPosition.y / 192.0f) * (BOX_WIDTH_N / 2.0f);
  lightSource_n.z = boxMidZ_n;
  glm::vec3 lightSource_f;
  lightSource_f.x = boxMidX_f + (sunDeltaX / (float)WORLD_WIDTH) * BOX_WIDTH_F;
  lightSource_f.y = (sunPosition.y / 192.0f) * (BOX_WIDTH_F / 2.0f);
  lightSource_f.z = boxMidZ_f;
  //step 6 - calculate frustum borders
  float frustumRight = glm::abs(boxNegZ_n - boxMidZ_n);
  float frustumLeft = -frustumRight;
  float frustumTop = glm::abs(boxNegX_n - boxMidX_n);
  float frustumBottom = -frustumTop;
  float frustumNear = 0.1f;
  float frustumFar = BOX_WIDTH_N;
  float farFrustumRight = glm::abs(boxNegZ_f - boxMidZ_f);
  float farFrustumLeft = -farFrustumRight;
  float farFrustumTop = glm::abs(boxNegX_f - boxMidX_f);
  float farFrustumBottom = -farFrustumTop;
  float farFrustumNear = 0.1f;
  float farFrustumFar = BOX_WIDTH_F;

  glm::mat4 lightProjectionNear = glm::ortho(isPosX ? frustumRight : frustumLeft,
                                             isPosX ? frustumLeft : frustumRight,
                                             isPosX ? frustumTop : frustumBottom,
                                             isPosX ? frustumBottom : frustumTop,
                                             frustumNear,
                                             frustumFar);
  glm::mat4 lightViewNear = glm::lookAt(lightSource_n, lightSource_n + lightDirTo, glm::vec3(0.0f, 1.0f, 0.0f));
  lightSpaceMatrixNear = lightProjectionNear * lightViewNear;

  glm::mat4 lightProjectionFar = glm::ortho(isPosX ? farFrustumRight : farFrustumLeft,
                                            isPosX ? farFrustumLeft : farFrustumRight,
                                            isPosX ? farFrustumTop : farFrustumBottom,
                                            isPosX ? farFrustumBottom : farFrustumTop,
                                            farFrustumNear,
                                            farFrustumFar);
  glm::mat4 lightViewFar = glm::lookAt(lightSource_f, lightSource_f + lightDirTo, glm::vec3(0.0f, 1.0f, 0.0f));
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
