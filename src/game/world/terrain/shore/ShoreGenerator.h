/*
 * Copyright 2019 Ilya Malgin
 * ShoreGenerator.h
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
 * Purpose: contains declaration for ShoreGenerator class and ShoreVertex struct
 * @version 0.1.0
 */

#pragma once

#include "Generator"

#include <random>

/**
* @brief generator for shore data on world map. This one has additional normal map storage for smooth shading
*/
class ShoreGenerator : public Generator
{
public:
  ShoreGenerator(const map2D_f& waterMap);
  void setup();

private:
  friend class ShoreRenderer;
  friend class ShoreFacade;

  /**
  * @brief representation of a shore vertex as it is in the shore shader
  */
  struct ShoreVertex
  {
    constexpr static unsigned int NUMBER_OF_ELEMENTS = 8;
    ShoreVertex(glm::vec3 position, glm::vec2 texCoords, glm::vec3 normal) noexcept;
	struct
	{
		float x, y, z;
	} position;
	struct
	{
		float x, y;
	} texCoords;
	struct
	{
		float x, y, z;
	} normal;
  };

  void generateMap();
  void shapeShoreProfile();
  void randomizeShore();
  void correctMapAtEdges();
  void applySlopeToProfile(float ratio) noexcept;
  void removeUnderwaterTiles(float thresholdValue);
  void createTiles();
  void fillBufferData();
  void bufferVertex(GLfloat* vertices, int offset, ShoreVertex vertex) noexcept;

  const map2D_f& waterMap;
  map2D_vec3 normalMap;
  std::default_random_engine randomizer;
};
