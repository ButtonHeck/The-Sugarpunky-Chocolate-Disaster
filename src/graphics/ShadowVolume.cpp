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
    updateBox(frustums[layer], layer, sunAbsPositionY, sunAbsPositionX);
}

glm::vec3 ShadowVolume::getLightDir() const
{
  return lightDirTo;
}

const std::array<glm::mat4, NUM_SHADOW_LAYERS> &ShadowVolume::getLightSpaceMatrices() const
{
  return lightSpaceMatrices;
}

void ShadowVolume::updateBox(const Frustum &frustum, int layer, float sunAbsPositionY, float sunAbsPositionX)
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
      boxMaxX = glm::min(HALF_WORLD_WIDTH_F, boxMaxX);
    }
  else
    {
      offset = glm::mix(-30.0f, 0.0f, sunAbsPositionY);
      boxMinX += offset;
      boxMinX = glm::max(-HALF_WORLD_WIDTH_F, boxMinX);
    }

  //step 3 - calculate box center
  float boxWidth = boxMaxX - boxMinX;
  float boxMidX = (boxMaxX + boxMinX) * 0.5f;
  float boxMidZ = (boxMaxZ + boxMinZ) * 0.5f;

  //step 4 - calculate light source position
  glm::vec3 lightSource;
  lightSource.x = boxMidX + sunAbsPositionX * (boxWidth * 0.5f);
  lightSource.y = sunAbsPositionY * (boxWidth * 0.5f);
  lightSource.z = boxMidZ;

  //step 5 - calculate frustum borders
  float frustumRight = glm::abs(boxMinZ - boxMidZ);
  float frustumLeft = -frustumRight;
  float frustumTop = glm::abs(boxMinX - boxMidX);
  float frustumBottom = -frustumTop;
  float frustumNear = 0.1f;
  float frustumFar = boxWidth;

  //step 6 - calculate light space matrix
  bool isLightDirPosX = lightDirTo.x > 0.0f;
  glm::mat4 projection = glm::ortho(!isLightDirPosX ? frustumRight : frustumLeft,
                                         !isLightDirPosX ? frustumLeft : frustumRight,
                                         !isLightDirPosX ? frustumTop : frustumBottom,
                                         !isLightDirPosX ? frustumBottom : frustumTop,
                                         frustumNear,
                                         frustumFar);
  glm::mat4 view = glm::lookAt(lightSource, lightSource + lightDirTo, glm::vec3(0.0f, 1.0f, 0.0f));
  lightSpaceMatrices[layer] = projection * view;

  //step 7 - update box struct
  shadowBoxes[layer].ll = glm::vec2(boxMinX, boxMaxZ);
  shadowBoxes[layer].lr = glm::vec2(boxMaxX, boxMaxZ);
  shadowBoxes[layer].ur = glm::vec2(boxMaxX, boxMinZ);
  shadowBoxes[layer].ul = glm::vec2(boxMinX, boxMinZ);
  shadowBoxes[layer].lightSource = lightSource;
  shadowBoxes[layer].nearLL = lightSource + (frustumNear * lightDirTo) + (frustumLeft * lightDirRight)  + (frustumBottom * lightDirUp);
  shadowBoxes[layer].nearLR = lightSource + (frustumNear * lightDirTo) + (frustumRight * lightDirRight) + (frustumBottom * lightDirUp);
  shadowBoxes[layer].nearUR = lightSource + (frustumNear * lightDirTo) + (frustumRight * lightDirRight) + (frustumTop * lightDirUp);
  shadowBoxes[layer].nearUL = lightSource + (frustumNear * lightDirTo) + (frustumLeft * lightDirRight)  + (frustumTop * lightDirUp);
  shadowBoxes[layer].farLL =  lightSource + (frustumFar * lightDirTo)  + (frustumLeft * lightDirRight)  + (frustumBottom * lightDirUp);
  shadowBoxes[layer].farLR =  lightSource + (frustumFar * lightDirTo)  + (frustumRight * lightDirRight) + (frustumBottom * lightDirUp);
  shadowBoxes[layer].farUR =  lightSource + (frustumFar * lightDirTo)  + (frustumRight * lightDirRight) + (frustumTop * lightDirUp);
  shadowBoxes[layer].farUL =  lightSource + (frustumFar * lightDirTo)  + (frustumLeft * lightDirRight)  + (frustumTop * lightDirUp);
}
