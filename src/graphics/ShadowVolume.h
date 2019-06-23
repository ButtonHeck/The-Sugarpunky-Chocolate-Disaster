/*
 * Copyright 2019 Ilya Malgin
 * ShadowVolume.h
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
 * Purpose: contains declaration for ShadowVolume class and ShadowBox struct
 * @version 0.1.0
 */

#pragma once

#include "GraphicsSettings"

#include <array>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class TheSunFacade;
class Frustum;

/**
* @brief representation of the shadow volume in the game. Shadow volume is a set of three boxes whose dimension and
* position parameters depend on a camera position and orientation, these boxes defines shadow regions of the current frame
* starting from the most detailed one (closest to the camera) to the least detailed (the farthest one).
* Based on frustums positions and orientations shadow volume calculates shadow regions
* and then calculates a set of projection*view matrices (from the sun point of view)
*/
class ShadowVolume
{
public:
  /** 
  * @todo debug visualization constant, remove in release version of the game 
  * @see ShadowVolumeRenderer
  */
  constexpr static unsigned int BOX_EXPECTED_VERTICES = 4;
  /**
  * @todo debug visualization constant, remove in release version of the game
  * @see ShadowVolumeRenderer
  */
  constexpr static unsigned int BOX_ACTUAL_VERTICES = 8;
  /** @brief minimal height of the shadow region box */
  constexpr static float BOX_MIN_HEIGHT = 14.0f;

  ShadowVolume() = default;
  void update(const std::array<Frustum, NUM_SHADOW_LAYERS>& frustums, const TheSunFacade &theSunFacade);
  const std::array<glm::mat4, NUM_SHADOW_LAYERS> &getLightSpaceMatrices() const noexcept;

private:
  /** 
  * @brief additional maximum offset for a region on X axis, 
  * need it to include some out-of-camera objects whose shadows should be visible 
  */
  constexpr static float SHADOW_BOX_MAX_OFFSET_X = 30.0f;
  /**
  * @brief additional offset for a region that is close to border,
  * need it to mitigate shadowing artefacts on the edge of world map
  */
  constexpr static float SHADOW_BOX_MAP_BORDER_OFFSET = 0.5f;
  const float SHADOW_BOXES_MIN_HEIGHT[NUM_SHADOW_LAYERS] = {BOX_MIN_HEIGHT, BOX_MIN_HEIGHT * 2, BOX_MIN_HEIGHT * 4};

  /**
  * @brief represents a shadow region box, used only for debug visualization
  * @note lsPov - light source point of view
  * @todo remove this in release version of the game
  */
  struct Box
  {
    glm::vec2 expectedLL, expectedLR, expectedUR, expectedUL;
    glm::vec3 lsPovNearLL, lsPovNearLR, lsPovNearUR, lsPovNearUL;
    glm::vec3 lsPovFarLL, lsPovFarLR, lsPovFarUR, lsPovFarUL;
    glm::vec3 localLightSource;
  };
  friend class ShadowVolumeRenderer;

  void updateLightSpaceMatrix(const Frustum& frustum, int layer, float sunAbsPositionY, float sunAbsPositionX);

  glm::vec3 lightDirTo, lightDirRight, lightDirUp;
  std::array<glm::mat4, 3> lightSpaceMatrices;
  std::array<Box, NUM_SHADOW_LAYERS> shadowBoxes;
};
