#include "ShadowVolume"

void ShadowVolume::update(const std::array<Frustum, NUM_SHADOW_LAYERS> &frustums, const TheSunFacade& theSunFacade)
{
  glm::vec3 sunPosition = theSunFacade.getPosition();

  //map sun position coordinates to [-1;1] assuming that the sun move trajectory is a circle
  float sunAbsPositionY = sunPosition.y / HALF_WORLD_WIDTH_F;
  float sunAbsPositionX = sunPosition.x / HALF_WORLD_WIDTH_F;

  lightDirTo = theSunFacade.getLightDir();
  lightDirRight = glm::normalize(glm::cross(lightDirTo, glm::vec3(0.0f, 1.0f, 0.0f)));
  /* for "up" vector no need to normalize it explicitly
   * as the cross product of 2 orthogonal unit vectors (lightDirRight and Y-axis unit vector) is already a unit length vector
   */
  lightDirUp = glm::cross(lightDirRight, lightDirTo);

  for (unsigned int layerIndex = 0; layerIndex < NUM_SHADOW_LAYERS; layerIndex++)
    updateLightSpaceMatrix(frustums[layerIndex], layerIndex, sunAbsPositionY, sunAbsPositionX);
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
  float boxMinZ = frustum.getMinCoordZ();

  //step 2 - offset bounding box position relative to where the sun is
  float offset = glm::mix(SHADOW_BOX_MAX_OFFSET_X, 0.0f, sunAbsPositionY);
  if (lightDirTo.x < 0.0f)
    boxMaxX += offset;
  else
    boxMinX -= offset;

  //limit box bounds with offsetted map bounds to hide shadow artefacts at map edges
  boxMaxZ = glm::min(HALF_WORLD_HEIGHT_F + SHADOW_BOX_MAP_BORDER_OFFSET, boxMaxZ);
  boxMinZ = glm::max(-HALF_WORLD_HEIGHT_F - SHADOW_BOX_MAP_BORDER_OFFSET, boxMinZ);
  boxMaxX = glm::min(HALF_WORLD_WIDTH_F + SHADOW_BOX_MAP_BORDER_OFFSET, boxMaxX);
  boxMinX = glm::max(-HALF_WORLD_WIDTH_F - SHADOW_BOX_MAP_BORDER_OFFSET, boxMinX);

  //step 3 - calculate box center
  float boxWidth = boxMaxX - boxMinX;
  float boxHalfWidth = boxWidth * 0.5f;
  float boxMidX = (boxMaxX + boxMinX) * 0.5f;
  float boxMidZ = (boxMaxZ + boxMinZ) * 0.5f;

  //step 4 - calculate light source position
  float angleRad = glm::atan(SHADOW_BOXES_MAX_HEIGHT[layer] / boxHalfWidth);
  float ellipseA = boxHalfWidth / glm::cos(angleRad);
  float ellipseB = SHADOW_BOXES_MAX_HEIGHT[layer] / glm::sin(angleRad);

  glm::vec3 lightSource(boxMidX + sunAbsPositionX * ellipseA,
                        sunAbsPositionY * ellipseB,
                        boxMidZ);

  //step 5 - calculate frustum borders
  const float RIGHT_SIDE = glm::abs(boxMinZ - boxMidZ);
  const float LEFT_SIDE = -RIGHT_SIDE;
  glm::vec2 lightSourceXY = glm::vec2(lightSource);
  glm::vec2 lightDirToXY = glm::vec2(lightDirTo);
  glm::vec2 lightDirUpXY = glm::vec2(lightDirUp);

  glm::vec2 farPoint(lightDirTo.x > 0.0f ? boxMaxX : boxMinX, 0.0f);
  glm::vec2 fromLStoFarPoint = farPoint - lightSourceXY;
  float fromLStoFarPointLength = glm::length(fromLStoFarPoint);
  glm::vec2 fromLStoFarPointNorm = glm::normalize(fromLStoFarPoint);
  float cosAlpha = glm::dot(fromLStoFarPointNorm, lightDirToXY);
  const float FAR_SIDE = fromLStoFarPointLength * cosAlpha
                          + 4.0f * sunAbsPositionY; //+some offset when the sun is at its zenith

  glm::vec2 nearPoint(lightDirTo.x > 0.0f ? boxMinX : boxMaxX, SHADOW_BOXES_MAX_HEIGHT[layer]);
  glm::vec2 fromLStoNearPoint = nearPoint - lightSourceXY;
  float fromLStoNearPointLength = glm::length(fromLStoNearPoint);
  glm::vec2 fromLStoNearPointNorm = glm::normalize(fromLStoNearPoint);
  cosAlpha = glm::dot(fromLStoNearPointNorm, lightDirToXY);
  const float NEAR_SIDE = fromLStoNearPointLength * cosAlpha;

  glm::vec2 upPoint(lightDirTo.x > 0.0f ? boxMaxX : boxMinX, SHADOW_BOXES_MAX_HEIGHT[layer]);
  glm::vec2 fromLStoUpPoint = upPoint - lightSourceXY;
  float fromLStoUpPointLength = glm::length(fromLStoUpPoint);
  glm::vec2 fromLStoUpPointNorm = glm::normalize(fromLStoUpPoint);
  cosAlpha = glm::dot(fromLStoUpPointNorm, lightDirUpXY);
  const float UP_SIDE = fromLStoUpPointLength * cosAlpha;

  glm::vec2 bottomPoint(lightDirTo.x > 0.0f ? boxMinX : boxMaxX, 0.0f);
  glm::vec2 fromLStoBottomPoint = bottomPoint - lightSourceXY;
  float fromLStoBottomPointLength = glm::length(fromLStoBottomPoint);
  glm::vec2 fromLStoBottomPointNorm = glm::normalize(fromLStoBottomPoint);
  cosAlpha = glm::dot(fromLStoBottomPointNorm, -lightDirUpXY);
  /*
   * for this we should consider additional offset, because lowest shore fragments are occured below 0.0 level,
   * thus the offset must be no less than lowest shore fragment. But we need this offset during only dusk and dawn
   */
  const float BOTTOM_SIDE = -fromLStoBottomPointLength * cosAlpha
                            - 5.0f * (1.0f - sunAbsPositionY);

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
  shadowBoxes[layer].expectedLL = glm::vec2(boxMinX, boxMaxZ);
  shadowBoxes[layer].expectedLR = glm::vec2(boxMaxX, boxMaxZ);
  shadowBoxes[layer].expectedUR = glm::vec2(boxMaxX, boxMinZ);
  shadowBoxes[layer].expectedUL = glm::vec2(boxMinX, boxMinZ);
  shadowBoxes[layer].localLightSource = lightSource;
  shadowBoxes[layer].lsPovNearLL = lightSource + (NEAR_SIDE * lightDirTo) + (LEFT_SIDE * lightDirRight)  + (BOTTOM_SIDE * lightDirUp);
  shadowBoxes[layer].lsPovNearLR = lightSource + (NEAR_SIDE * lightDirTo) + (RIGHT_SIDE * lightDirRight) + (BOTTOM_SIDE * lightDirUp);
  shadowBoxes[layer].lsPovNearUR = lightSource + (NEAR_SIDE * lightDirTo) + (RIGHT_SIDE * lightDirRight) + (UP_SIDE * lightDirUp);
  shadowBoxes[layer].lsPovNearUL = lightSource + (NEAR_SIDE * lightDirTo) + (LEFT_SIDE * lightDirRight)  + (UP_SIDE * lightDirUp);
  shadowBoxes[layer].lsPovFarLL =  lightSource + (FAR_SIDE * lightDirTo)  + (LEFT_SIDE * lightDirRight)  + (BOTTOM_SIDE * lightDirUp);
  shadowBoxes[layer].lsPovFarLR =  lightSource + (FAR_SIDE * lightDirTo)  + (RIGHT_SIDE * lightDirRight) + (BOTTOM_SIDE * lightDirUp);
  shadowBoxes[layer].lsPovFarUR =  lightSource + (FAR_SIDE * lightDirTo)  + (RIGHT_SIDE * lightDirRight) + (UP_SIDE * lightDirUp);
  shadowBoxes[layer].lsPovFarUL =  lightSource + (FAR_SIDE * lightDirTo)  + (LEFT_SIDE * lightDirRight)  + (UP_SIDE * lightDirUp);
}
