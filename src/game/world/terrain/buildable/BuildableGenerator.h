/*
 * Copyright 2019 Ilya Malgin
 * BuildableGenerator.h
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
 * Purpose: contains declaration for BuildableGenerator class
 * @version 0.1.0
 */

#pragma once

#include "Generator"

/**
* @brief Generator for buildable segments of the world map. Technically, it doesn't "generate" anything,
* but rather checks which tiles of already created map are available for building on them. Additionally
* it is holding a buffer collection for currently selected tile
*/
class BuildableGenerator : public Generator
{
public:
  BuildableGenerator() noexcept;
  virtual ~BuildableGenerator() = default;
  void setup(const map2D_f &landMap, const map2D_f &hillsMap);
  void createTiles();

private:
  friend class BuildableRenderer;
  const unsigned int UPPER_LEFT_CORNER_START_Y = 2;
  const unsigned int UPPER_LEFT_CORNER_START_X = 1;

  void setupAndBindBuffers(BufferCollection& buffers);
  void fillBufferData();

  BufferCollection selectedBuffers;
};
