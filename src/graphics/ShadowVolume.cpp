#include "ShadowVolume.h"

ShadowVolume::ShadowVolume(TheSunFacade& sun)
  :
    sun(sun)
{}

void ShadowVolume::update(const std::array<Frustum, NUM_SHADOW_LAYERS> &frustums)
{
  glm::vec3 sunPosition = sun.getCurrentPosition();
  lightDirTo = glm::normalize(glm::vec3(0.0f) - sunPosition);
  glm::vec3 lightDirRight = glm::normalize(glm::cross(lightDirTo, glm::vec3(0.0f, 1.0f, 0.0f)));
  glm::vec3 lightDirUp = glm::normalize(glm::cross(lightDirRight, lightDirTo));
  bool isPosX = sunPosition.x > 0.0f;

  //step 1 - calculate bounding boxes bounds
  float boxPosX_n = std::max({frustums[0].nearLL.x,
                              frustums[0].nearLR.x,
                              frustums[0].nearUR.x,
                              frustums[0].nearUL.x,
                              frustums[0].farLL.x,
                              frustums[0].farLR.x,
                              frustums[0].farUR.x,
                              frustums[0].farUL.x});
  float boxNegX_n = std::min({frustums[0].nearLL.x,
                              frustums[0].nearLR.x,
                              frustums[0].nearUR.x,
                              frustums[0].nearUL.x,
                              frustums[0].farLL.x,
                              frustums[0].farLR.x,
                              frustums[0].farUR.x,
                              frustums[0].farUL.x});
  float boxPosZ_n = std::max({frustums[0].nearLL.z,
                              frustums[0].nearLR.z,
                              frustums[0].nearUR.z,
                              frustums[0].nearUL.z,
                              frustums[0].farLL.z,
                              frustums[0].farLR.z,
                              frustums[0].farUR.z,
                              frustums[0].farUL.z});
  float boxNegZ_n = std::min({frustums[0].nearLL.z,
                              frustums[0].nearLR.z,
                              frustums[0].nearUR.z,
                              frustums[0].nearUL.z,
                              frustums[0].farLL.z,
                              frustums[0].farLR.z,
                              frustums[0].farUR.z,
                              frustums[0].farUL.z});

  float boxPosX_m = std::max({frustums[1].nearLL.x,
                              frustums[1].nearLR.x,
                              frustums[1].nearUR.x,
                              frustums[1].nearUL.x,
                              frustums[1].farLL.x,
                              frustums[1].farLR.x,
                              frustums[1].farUR.x,
                              frustums[1].farUL.x});
  float boxNegX_m = std::min({frustums[1].nearLL.x,
                              frustums[1].nearLR.x,
                              frustums[1].nearUR.x,
                              frustums[1].nearUL.x,
                              frustums[1].farLL.x,
                              frustums[1].farLR.x,
                              frustums[1].farUR.x,
                              frustums[1].farUL.x});
  float boxPosZ_m = std::max({frustums[1].nearLL.z,
                              frustums[1].nearLR.z,
                              frustums[1].nearUR.z,
                              frustums[1].nearUL.z,
                              frustums[1].farLL.z,
                              frustums[1].farLR.z,
                              frustums[1].farUR.z,
                              frustums[1].farUL.z});
  float boxNegZ_m = std::min({frustums[1].nearLL.z,
                              frustums[1].nearLR.z,
                              frustums[1].nearUR.z,
                              frustums[1].nearUL.z,
                              frustums[1].farLL.z,
                              frustums[1].farLR.z,
                              frustums[1].farUR.z,
                              frustums[1].farUL.z});

  float boxPosX_f = std::max({frustums[2].nearLL.x,
                              frustums[2].nearLR.x,
                              frustums[2].nearUR.x,
                              frustums[2].nearUL.x,
                              frustums[2].farLL.x,
                              frustums[2].farLR.x,
                              frustums[2].farUR.x,
                              frustums[2].farUL.x});
  float boxNegX_f = std::min({frustums[2].nearLL.x,
                              frustums[2].nearLR.x,
                              frustums[2].nearUR.x,
                              frustums[2].nearUL.x,
                              frustums[2].farLL.x,
                              frustums[2].farLR.x,
                              frustums[2].farUR.x,
                              frustums[2].farUL.x});
  float boxPosZ_f = std::max({frustums[2].nearLL.z,
                              frustums[2].nearLR.z,
                              frustums[2].nearUR.z,
                              frustums[2].nearUL.z,
                              frustums[2].farLL.z,
                              frustums[2].farLR.z,
                              frustums[2].farUR.z,
                              frustums[2].farUL.z});
  float boxNegZ_f = std::min({frustums[2].nearLL.z,
                              frustums[2].nearLR.z,
                              frustums[2].nearUR.z,
                              frustums[2].nearUL.z,
                              frustums[2].farLL.z,
                              frustums[2].farLR.z,
                              frustums[2].farUR.z,
                              frustums[2].farUL.z});

  //step 2 - correct bounding box
  float offset;
  if (lightDirTo.x < 0.0f)
    {
      offset = glm::mix(30.0f, 0.0f, sunPosition.y / 192.0f);
      boxPosX_n += offset;
      boxPosX_n = glm::min(192.0f, boxPosX_n);
      boxPosX_m += offset;
      boxPosX_m = glm::min(192.0f, boxPosX_m);
      boxPosX_f += offset;
      boxPosX_f = glm::min(192.0f, boxPosX_f);
    }
  else
    {
      offset = glm::mix(-30.0f, 0.0f, sunPosition.y / 192.0f);
      boxNegX_n += offset;
      boxNegX_n = glm::max(-192.0f, boxNegX_n);
      boxNegX_m += offset;
      boxNegX_m = glm::max(-192.0f, boxNegX_m);
      boxNegX_f += offset;
      boxNegX_f = glm::max(-192.0f, boxNegX_f);
    }

  //step 3 - check sunPosition
  float sunDeltaX = sunPosition.x / 192.0f;

  //step 4 - calculate box center
  float BOX_WIDTH_N = boxPosX_n - boxNegX_n;
  float boxMidX_n = (boxPosX_n + boxNegX_n) / 2.0f;
  float boxMidZ_n = (boxPosZ_n + boxNegZ_n) / 2.0f;
  float BOX_WIDTH_M = boxPosX_m - boxNegX_m;
  float boxMidX_m = (boxPosX_m + boxNegX_m) / 2.0f;
  float boxMidZ_m = (boxPosZ_m + boxNegZ_m) / 2.0f;
  float BOX_WIDTH_F = boxPosX_f - boxNegX_f;
  float boxMidX_f = (boxPosX_f + boxNegX_f) / 2.0f;
  float boxMidZ_f = (boxPosZ_f + boxNegZ_f) / 2.0f;

  //step 5 - calculate light source position
  glm::vec3 lightSource_n;
  lightSource_n.x = boxMidX_n + sunDeltaX * BOX_WIDTH_N / 2.0f;
  lightSource_n.y = (sunPosition.y / 192.0f) * (BOX_WIDTH_N / 2.0f);
  lightSource_n.z = boxMidZ_n;
  glm::vec3 lightSource_m;
  lightSource_m.x = boxMidX_m + sunDeltaX * BOX_WIDTH_M / 2.0f;
  lightSource_m.y = (sunPosition.y / 192.0f) * (BOX_WIDTH_M / 2.0f);
  lightSource_m.z = boxMidZ_m;
  glm::vec3 lightSource_f;
  lightSource_f.x = boxMidX_f + sunDeltaX * BOX_WIDTH_F / 2.0f;
  lightSource_f.y = (sunPosition.y / 192.0f) * (BOX_WIDTH_F / 2.0f);
  lightSource_f.z = boxMidZ_f;

  //step 6 - calculate frustum borders
  float frustumRight_n = glm::abs(boxNegZ_n - boxMidZ_n);
  float frustumLeft_n = -frustumRight_n;
  float frustumTop_n = glm::abs(boxNegX_n - boxMidX_n);
  float frustumBottom_n = -frustumTop_n;
  float frustumNear_n = 0.1f;
  float frustumFar_n = BOX_WIDTH_N;

  float frustumRight_m = glm::abs(boxNegZ_m - boxMidZ_m);
  float frustumLeft_m = -frustumRight_m;
  float frustumTop_m = glm::abs(boxNegX_m - boxMidX_m);
  float frustumBottom_m = -frustumTop_m;
  float frustumNear_m = 0.1f;
  float frustumFar_m = BOX_WIDTH_M;

  float frustumRight_f = glm::abs(boxNegZ_f - boxMidZ_f);
  float frustumLeft_f = -frustumRight_f;
  float frustumTop_f = glm::abs(boxNegX_f - boxMidX_f);
  float frustumBottom_f = -frustumTop_f;
  float frustumNear_f = 0.1f;
  float frustumFar_f = BOX_WIDTH_F;

  glm::mat4 lightProjectionNear = glm::ortho(isPosX ? frustumRight_n : frustumLeft_n,
                                             isPosX ? frustumLeft_n : frustumRight_n,
                                             isPosX ? frustumTop_n : frustumBottom_n,
                                             isPosX ? frustumBottom_n : frustumTop_n,
                                             frustumNear_n,
                                             frustumFar_n);
  glm::mat4 lightViewNear = glm::lookAt(lightSource_n, lightSource_n + lightDirTo, glm::vec3(0.0f, 1.0f, 0.0f));
  lightSpaceMatrices[0] = lightProjectionNear * lightViewNear;

  glm::mat4 lightProjectionMiddle = glm::ortho(isPosX ? frustumRight_m : frustumLeft_m,
                                               isPosX ? frustumLeft_m : frustumRight_m,
                                               isPosX ? frustumTop_m : frustumBottom_m,
                                               isPosX ? frustumBottom_m : frustumTop_m,
                                               frustumNear_m,
                                               frustumFar_m);
  glm::mat4 lightViewMiddle = glm::lookAt(lightSource_m, lightSource_m + lightDirTo, glm::vec3(0.0f, 1.0f, 0.0f));
  lightSpaceMatrices[1] = lightProjectionMiddle * lightViewMiddle;

  glm::mat4 lightProjectionFar = glm::ortho(isPosX ? frustumRight_f : frustumLeft_f,
                                            isPosX ? frustumLeft_f : frustumRight_f,
                                            isPosX ? frustumTop_f : frustumBottom_f,
                                            isPosX ? frustumBottom_f : frustumTop_f,
                                            frustumNear_f,
                                            frustumFar_f);
  glm::mat4 lightViewFar = glm::lookAt(lightSource_f, lightSource_f + lightDirTo, glm::vec3(0.0f, 1.0f, 0.0f));
  lightSpaceMatrices[2] = lightProjectionFar * lightViewFar;

  nearBox.ll = glm::vec2(boxNegX_n, boxPosZ_n);
  nearBox.lr = glm::vec2(boxPosX_n, boxPosZ_n);
  nearBox.ur = glm::vec2(boxPosX_n, boxNegZ_n);
  nearBox.ul = glm::vec2(boxNegX_n, boxNegZ_n);
  nearBox.lightSource = lightSource_n;
  nearBox.nearLL = lightSource_n + (frustumNear_n * lightDirTo) + (frustumLeft_n * lightDirRight) + (frustumBottom_n * lightDirUp);
  nearBox.nearLR = lightSource_n + (frustumNear_n * lightDirTo) + (frustumRight_n * lightDirRight) + (frustumBottom_n * lightDirUp);
  nearBox.nearUR = lightSource_n + (frustumNear_n * lightDirTo) + (frustumRight_n * lightDirRight) + (frustumTop_n * lightDirUp);
  nearBox.nearUL = lightSource_n + (frustumNear_n * lightDirTo) + (frustumLeft_n * lightDirRight) + (frustumTop_n * lightDirUp);
  nearBox.farLL = lightSource_n + (frustumFar_n * lightDirTo) + (frustumLeft_n * lightDirRight) + (frustumBottom_n * lightDirUp);
  nearBox.farLR = lightSource_n + (frustumFar_n * lightDirTo) + (frustumRight_n * lightDirRight) + (frustumBottom_n * lightDirUp);
  nearBox.farUR = lightSource_n + (frustumFar_n * lightDirTo) + (frustumRight_n * lightDirRight) + (frustumTop_n * lightDirUp);
  nearBox.farUL = lightSource_n + (frustumFar_n * lightDirTo) + (frustumLeft_n * lightDirRight) + (frustumTop_n * lightDirUp);

  middleBox.ll = glm::vec2(boxNegX_m, boxPosZ_m);
  middleBox.lr = glm::vec2(boxPosX_m, boxPosZ_m);
  middleBox.ur = glm::vec2(boxPosX_m, boxNegZ_m);
  middleBox.ul = glm::vec2(boxNegX_m, boxNegZ_m);
  middleBox.lightSource = lightSource_m;
  middleBox.nearLL = lightSource_m + (frustumNear_m * lightDirTo) + (frustumLeft_m * lightDirRight) + (frustumBottom_m * lightDirUp);
  middleBox.nearLR = lightSource_m + (frustumNear_m * lightDirTo) + (frustumRight_m * lightDirRight) + (frustumBottom_m * lightDirUp);
  middleBox.nearUR = lightSource_m + (frustumNear_m * lightDirTo) + (frustumRight_m * lightDirRight) + (frustumTop_m * lightDirUp);
  middleBox.nearUL = lightSource_m + (frustumNear_m * lightDirTo) + (frustumLeft_m * lightDirRight) + (frustumTop_m * lightDirUp);
  middleBox.farLL = lightSource_m + (frustumFar_m * lightDirTo) + (frustumLeft_m * lightDirRight) + (frustumBottom_m * lightDirUp);
  middleBox.farLR = lightSource_m + (frustumFar_m * lightDirTo) + (frustumRight_m * lightDirRight) + (frustumBottom_m * lightDirUp);
  middleBox.farUR = lightSource_m + (frustumFar_m * lightDirTo) + (frustumRight_m * lightDirRight) + (frustumTop_m * lightDirUp);
  middleBox.farUL = lightSource_m + (frustumFar_m * lightDirTo) + (frustumLeft_m * lightDirRight) + (frustumTop_m * lightDirUp);

  farBox.ll = glm::vec2(boxNegX_f, boxPosZ_f);
  farBox.lr = glm::vec2(boxPosX_f, boxPosZ_f);
  farBox.ur = glm::vec2(boxPosX_f, boxNegZ_f);
  farBox.ul = glm::vec2(boxNegX_f, boxNegZ_f);
  farBox.lightSource = lightSource_f;
  farBox.nearLL = lightSource_f + (frustumNear_f * lightDirTo) + (frustumLeft_f * lightDirRight) + (frustumBottom_f * lightDirUp);
  farBox.nearLR = lightSource_f + (frustumNear_f * lightDirTo) + (frustumRight_f * lightDirRight) + (frustumBottom_f * lightDirUp);
  farBox.nearUR = lightSource_f + (frustumNear_f * lightDirTo) + (frustumRight_f * lightDirRight) + (frustumTop_f * lightDirUp);
  farBox.nearUL = lightSource_f + (frustumNear_f * lightDirTo) + (frustumLeft_f * lightDirRight) + (frustumTop_f * lightDirUp);
  farBox.farLL = lightSource_f + (frustumFar_f * lightDirTo) + (frustumLeft_f * lightDirRight) + (frustumBottom_f * lightDirUp);
  farBox.farLR = lightSource_f + (frustumFar_f * lightDirTo) + (frustumRight_f * lightDirRight) + (frustumBottom_f * lightDirUp);
  farBox.farUR = lightSource_f + (frustumFar_f * lightDirTo) + (frustumRight_f * lightDirRight) + (frustumTop_f * lightDirUp);
  farBox.farUL = lightSource_f + (frustumFar_f * lightDirTo) + (frustumLeft_f * lightDirRight) + (frustumTop_f * lightDirUp);
}

glm::vec3 ShadowVolume::getLightDir() const
{
  return lightDirTo;
}

const std::array<glm::mat4, NUM_SHADOW_LAYERS> &ShadowVolume::getLightSpaceMatrices() const
{
  return lightSpaceMatrices;
}
