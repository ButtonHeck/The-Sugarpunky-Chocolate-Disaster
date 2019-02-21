/*
 * Copyright 2019 Ilya Malgin
 * TheSunFacade.cpp
 * This file is part of The Sugarpunky Chocolate Disaster project
 *
 * The Sugarpunky Chocolate Disaster project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Sugarpunky Chocolate Disaster project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * See <http://www.gnu.org/licenses/>
 *
 * Purpose: contains definitions for TheSunFacade class
 * @version 0.1.0
 */

#include "game/world/ambience/theSun/TheSunFacade.h"

/**
 * @brief initialize member variables, setup renderer point size and calculate maximum samples values
 * @param renderShader compiled shader program fed to personal shader manager
 * @param screenResolution current screen resolution to determine maximum possible samples to be drawn
 */
TheSunFacade::TheSunFacade(Shader &renderShader, const ScreenResolution &screenResolution)
  :
    theSun(),
    shader(renderShader),
    renderer(theSun)
{
  //calculate adjusted point size multiplier for plain rendering based on current screen resolution
  float pointSizeDivisorX = screenResolution.getWidthRatioToReference();
  float pointSizeDivisorY = screenResolution.getHeightRatioToReference();
  float pointSizeDivisor = (pointSizeDivisorX + pointSizeDivisorY) / 2;
  //calculate adjusted point size multiplier for world reflection rendering based on current screen resolution
  float reflectionPointSizeDivisorX = FRAME_WATER_REFLECTION_WIDTH / ScreenResolution::REFERENCE_WIDTH;
  float reflectionPointSizeDivisorY = FRAME_WATER_REFLECTION_HEIGHT / ScreenResolution::REFERENCE_HEIGHT;
  float reflectionPointSizeDivisor = (reflectionPointSizeDivisorX + reflectionPointSizeDivisorY) / 2;

  renderer.setPointSize(renderer.DEFAULT_SUN_POINT_SIZE * pointSizeDivisor);
  renderer.setReflectionPointSize(renderer.DEFAULT_SUN_POINT_SIZE * reflectionPointSizeDivisor);

  //and now set maximum samples passed values after renderer has been calculated actual point size
  float pointSize = renderer.getPointSize();
  maxSamplesPassed = pointSize * pointSize;
  maxSamplesPassedMultisampling = maxSamplesPassed * MULTISAMPLES;
}

/**
 * @brief debug_sunSpeed defined in main.cpp speed of the Sun
 * @todo TEST ONLY! make it constant member variable in game release version
 */
extern float debug_sunSpeed;

/**
 * @brief commands the Sun entity to move
 * @param angleRadians angle to rotate in degrees
 */
void TheSunFacade::move(float angleRadians)
{
  theSun.move(angleRadians * debug_sunSpeed);
}

/**
 * @brief commands the Sun entity to set its position
 * @param angleRadians angle to rotate in degrees
 * @todo TEST ONLY! remove this from the game release version
 */
void TheSunFacade::moveAbsolutePosition(float angleRadians)
{
  theSun.moveAbsolutePosition(angleRadians);
}

/**
 * @brief updates the shader and delegates draw call to renderer
 * @param skyProjectionView Projection*View matrix
 * @param doOcclusionTest define whether or not occlusion query takes place during rendering
 * @param useReflectionPointSize a flag for renderer indicating what point size to use
 */
void TheSunFacade::draw(const glm::mat4 &skyProjectionView, bool doOcclusionTest, bool useReflectionPointSize)
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

/**
 * @brief define how much the Sun current position would affect lens flare drawing routine
 * @param multisampled determines what maximum samples value to refer while calculating result
 * @param viewPositionY current camera's Y coordinate used to amplify visibility a bit when the
 * camera is high enough
 * @return ranged from 0.0 to 1.0 value of how much lens flare effect the Sun gives
 */
GLfloat TheSunFacade::getSunFlareBrightness(bool multisampled, float viewPositionY) const
{
  GLfloat visibility = renderer.getSamplesPassedQueryResult() / (multisampled ? maxSamplesPassedMultisampling : maxSamplesPassed);
  const float MIN_SUN_VISIBILITY_Y = 0.02f;
  const float VIEW_POSITION_VISIBILITY_DIVISOR = 1500.0f;
  visibility *= glm::clamp((-getLightDir().y - MIN_SUN_VISIBILITY_Y + viewPositionY / VIEW_POSITION_VISIBILITY_DIVISOR) * 8.0f, 0.0f, 1.0f);
  return visibility;
}
