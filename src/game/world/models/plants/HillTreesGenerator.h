/*
 * Copyright 2019 Ilya Malgin
 * HillTreesGenerator.h
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
 * Purpose: contains declaration for HillTreesGenerator class
 * @version 0.1.0
 */

#pragma once

#include "PlantGenerator"

/**
 * @brief Generator for plants located on hills.
 * Responsible for accomodating all hill plants models on the world map
 */
class HillTreesGenerator : public PlantGenerator
{
public:
  HillTreesGenerator();
  void setup(const map2D_f &hillMap, const map2D_i &distributionMap, const map2D_vec3 &hillsNormalMap);

private:
  const float MIN_SCALE_TREES = 0.38f;
  const float MAX_SCALE_TREES = 0.51f;
  const float MIN_SCALE_ROCKS = MIN_SCALE_TREES * 0.75f;
  const float MAX_SCALE_ROCKS = MAX_SCALE_TREES * 0.75f;
  const float MIN_POSITION_OFFSET = -0.6f;
  const float MAX_POSITION_OFFSET = 0.6f;
  const float MIN_ROTATION_OFFSET = -0.05f;
  const float MAX_ROTATION_OFFSET = 0.05f;
  const float MAX_SURFACE_SLOPE_FOR_TREES = 1.0f;
  const float MAX_SURFACE_SLOPE_FOR_ROCKS = 2.6f;

  void setupMatrices(const map2D_f &hillMap, const map2D_i &distributionMap, const map2D_vec3 &hillsNormalMap);

  //some models must have perpendicular orientation to a particular hill tile
  size_t numSurfaceOrientedModels;
};
