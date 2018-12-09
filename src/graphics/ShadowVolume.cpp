#include "ShadowVolume.h"

ShadowVolume::ShadowVolume(TheSunFacade& sun)
  :
    sun(sun)
{}

void ShadowVolume::update(const std::array<Frustum, NUM_SHADOW_LAYERS> &frustums)
{
  glm::vec3 sunPosition = sun.getCurrentPosition();
  float sunAbsPositionY = sunPosition.y / HALF_WORLD_WIDTH_F;
  float sunAbsPositionX = sunPosition.x / HALF_WORLD_WIDTH_F;
  lightDirTo = glm::normalize(glm::vec3(0.0f) - sunPosition);
  lightDirRight = glm::normalize(glm::cross(lightDirTo, glm::vec3(0.0f, 1.0f, 0.0f)));
  lightDirUp = glm::normalize(glm::cross(lightDirRight, lightDirTo));

  for (unsigned int layer = 0; layer < NUM_SHADOW_LAYERS; layer++)
    updateLightSpaceMatrix(frustums[layer], layer, sunAbsPositionY, sunAbsPositionX);
}

glm::vec3 ShadowVolume::getLightDir() const
{
  return lightDirTo;
}

const std::array<glm::mat4, NUM_SHADOW_LAYERS> &ShadowVolume::getLightSpaceMatrices() const
{
  return lightSpaceMatrices;
}

void ShadowVolume::updateLightSpaceMatrix(const Frustum &frustum, int layer, float sunAbsPositionY, float sunAbsPositionX)
{
  //step 1 - calculate bounding boxes bounds
  float boxMaxX = frustum.getMaxCoordX();
  float boxMinX = frustum.getMinCoordX();
  float boxMaxZ = frustum.getMaxCoordZ();
  boxMaxZ = glm::min(HALF_WORLD_HEIGHT_F, boxMaxZ);
  float boxMinZ = frustum.getMinCoordZ();
  boxMinZ = glm::max(-HALF_WORLD_HEIGHT_F, boxMinZ);

  //step 2 - correct bounding box
  float offset;
  if (lightDirTo.x < 0.0f)
    {
      offset = glm::mix(30.0f, 0.0f, sunAbsPositionY);
      boxMaxX += offset;
    }
  else
    {
      offset = glm::mix(-30.0f, 0.0f, sunAbsPositionY);
      boxMinX += offset;
    }
  boxMaxX = glm::min(HALF_WORLD_WIDTH_F, boxMaxX);
  boxMinX = glm::max(-HALF_WORLD_WIDTH_F, boxMinX);

  //step 3 - calculate box center
  float boxWidth = boxMaxX - boxMinX;
  float boxMidX = (boxMaxX + boxMinX) * 0.5f;
  float boxMidZ = (boxMaxZ + boxMinZ) * 0.5f;

  //step 4 - calculate light source position
  const float EXPECTED_MAX_HEIGHT = 14.0f;
  float x = boxWidth * 0.5f;
  float angleRad = glm::atan(EXPECTED_MAX_HEIGHT / x);
  float ellipseA = x / glm::cos(angleRad);
  float ellipseB = EXPECTED_MAX_HEIGHT / glm::sin(angleRad);

  glm::vec3 lightSource(boxMidX + sunAbsPositionX * ellipseA,
                        sunAbsPositionY * ellipseB,
                        boxMidZ);

  //step 5 - calculate frustum borders
  const float RIGHT_SIDE = glm::abs(boxMinZ - boxMidZ);
  const float LEFT_SIDE = -RIGHT_SIDE;

  glm::vec2 farPoint(lightDirTo.x > 0.0f ? boxMaxX : boxMinX, 0.0f);
  glm::vec2 fromLStoFarPoint = farPoint - glm::vec2(lightSource);
  float fromLStoFarPointLength = glm::length(fromLStoFarPoint);
  glm::vec2 fromLStoFarPointNorm = glm::normalize(fromLStoFarPoint);
  float cosAlpha = glm::dot(fromLStoFarPointNorm, glm::vec2(lightDirTo));
  const float FAR_SIDE = fromLStoFarPointLength * cosAlpha
                          + 4.0f * sunAbsPositionY; //+some offset when the sun is at its zenith

  glm::vec2 nearPoint(lightDirTo.x > 0.0f ? boxMinX : boxMaxX, EXPECTED_MAX_HEIGHT);
  glm::vec2 fromLStoNearPoint = nearPoint - glm::vec2(lightSource);
  float fromLStoNearPointLength = glm::length(fromLStoNearPoint);
  glm::vec2 fromLStoNearPointNorm = glm::normalize(fromLStoNearPoint);
  cosAlpha = glm::dot(fromLStoNearPointNorm, glm::vec2(lightDirTo));
  const float NEAR_SIDE = fromLStoNearPointLength * cosAlpha;

  glm::vec2 upPoint(lightDirTo.x > 0.0f ? boxMaxX : boxMinX, EXPECTED_MAX_HEIGHT);
  glm::vec2 fromLStoUpPoint = upPoint - glm::vec2(lightSource);
  float fromLStoUpPointLength = glm::length(fromLStoUpPoint);
  glm::vec2 fromLStoUpPointNorm = glm::normalize(fromLStoUpPoint);
  cosAlpha = glm::dot(fromLStoUpPointNorm, glm::vec2(lightDirUp));
  const float UP_SIDE = fromLStoUpPointLength * cosAlpha;

  glm::vec2 bottomPoint(lightDirTo.x > 0.0f ? boxMinX : boxMaxX, 0.0f);
  glm::vec2 fromLStoBottomPoint = bottomPoint - glm::vec2(lightSource);
  float fromLStoBottomPointLength = glm::length(fromLStoBottomPoint);
  glm::vec2 fromLStoBottomPointNorm = glm::normalize(fromLStoBottomPoint);
  cosAlpha = glm::dot(fromLStoBottomPointNorm, glm::vec2(-lightDirUp));
  const float BOTTOM_SIDE = -fromLStoBottomPointLength * cosAlpha
                            - 2.0f * (1.0f - sunAbsPositionY); //+some offset when the dusk or the dawn

  //step 6 - calculate light space matrix
  bool isLightDirPosX = lightDirTo.x > 0.0f;
  glm::mat4 projection = glm::ortho(!isLightDirPosX ? RIGHT_SIDE : LEFT_SIDE,
                                    !isLightDirPosX ? LEFT_SIDE : RIGHT_SIDE,
                                    !isLightDirPosX ? UP_SIDE : BOTTOM_SIDE,
                                    !isLightDirPosX ? BOTTOM_SIDE : UP_SIDE,
                                    NEAR_SIDE,
                                    FAR_SIDE);
  glm::mat4 view = glm::lookAt(lightSource, lightSource + lightDirTo, glm::vec3(0.0f, 1.0f, 0.0f));
  lightSpaceMatrices[layer] = projection * view;

  //step 7 - update box struct (optional for visualization)
  shadowBoxes[layer].ll = glm::vec2(boxMinX, boxMaxZ);
  shadowBoxes[layer].lr = glm::vec2(boxMaxX, boxMaxZ);
  shadowBoxes[layer].ur = glm::vec2(boxMaxX, boxMinZ);
  shadowBoxes[layer].ul = glm::vec2(boxMinX, boxMinZ);
  shadowBoxes[layer].lightSource = lightSource;
  shadowBoxes[layer].nearLL = lightSource + (NEAR_SIDE * lightDirTo) + (LEFT_SIDE * lightDirRight)  + (BOTTOM_SIDE * lightDirUp);
  shadowBoxes[layer].nearLR = lightSource + (NEAR_SIDE * lightDirTo) + (RIGHT_SIDE * lightDirRight) + (BOTTOM_SIDE * lightDirUp);
  shadowBoxes[layer].nearUR = lightSource + (NEAR_SIDE * lightDirTo) + (RIGHT_SIDE * lightDirRight) + (UP_SIDE * lightDirUp);
  shadowBoxes[layer].nearUL = lightSource + (NEAR_SIDE * lightDirTo) + (LEFT_SIDE * lightDirRight)  + (UP_SIDE * lightDirUp);
  shadowBoxes[layer].farLL =  lightSource + (FAR_SIDE * lightDirTo)  + (LEFT_SIDE * lightDirRight)  + (BOTTOM_SIDE * lightDirUp);
  shadowBoxes[layer].farLR =  lightSource + (FAR_SIDE * lightDirTo)  + (RIGHT_SIDE * lightDirRight) + (BOTTOM_SIDE * lightDirUp);
  shadowBoxes[layer].farUR =  lightSource + (FAR_SIDE * lightDirTo)  + (RIGHT_SIDE * lightDirRight) + (UP_SIDE * lightDirUp);
  shadowBoxes[layer].farUL =  lightSource + (FAR_SIDE * lightDirTo)  + (LEFT_SIDE * lightDirRight)  + (UP_SIDE * lightDirUp);
}
