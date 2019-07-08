/*
 * Copyright 2019 Ilya Malgin
 * WaterGenerator.h
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
 * Purpose: contains declaration for WaterGenerator class
 * @version 0.1.0
 */

#pragma once

#include "Generator"

#include <memory>

class WaterShader;

/**
* @brief generator for water on the world map.
* Responsible for creating water map, preparing buffer collections
*/
class WaterGenerator : public Generator
{
public:
  WaterGenerator(WaterShader& shaders);
  void setup();
  void setupConsiderTerrain();
  void createTiles();

private:
  constexpr static unsigned int RIVER_DIRECTION_CHANGE_DELAY = 48;

  friend class WaterRenderer;

  /**
  * @brief representation of a water vertex as it is in water shader
  */
  struct WaterVertex
  {
    constexpr static unsigned int NUMBER_OF_ELEMENTS = 4;
	const float X_POS_ANIM_MULTIPLIER = 15.11f;
	const float X_POS_ANIM_OFFSET = 121.197f;
    WaterVertex(glm::vec3 position, glm::vec2 animationOffset) noexcept;

	struct
	{
		float x, y, z;
	} position;
	float animationOffset;
  };

  enum DIRECTION : int {
      UP = 0, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT, NUM_DIRECTIONS
  };

  void generateMap();
  void expandWaterArea();
  void setNewDirection(unsigned int &curveDistanceStep,
                       unsigned int &curveMaxDistance,
                       DIRECTION &currentDirection,
                       DIRECTION validDirectionLeft,
                       DIRECTION validDirectionRight) noexcept;
  void fattenKernel(int x, int y, int &kernelCounter, int &riverWidthOffset, bool &riverWidthIncrease);
  void bufferVertex(GLfloat* vertices, int offset, WaterVertex vertex) noexcept;
  void setupVBOAttributes() noexcept;
  void fillBufferData();

  /** @note additional buffer collection containing data from transform feedback rendering */
  BufferCollection culledBuffers;
  WaterShader& shaders;
  size_t numVertices;
  size_t numTiles;
  std::unique_ptr<GLfloat[]> vertices;
  map2D_f postProcessMap;
};
