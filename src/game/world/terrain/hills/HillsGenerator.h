/*
 * Copyright 2019 Ilya Malgin
 * HillsGenerator.h
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
 * Purpose: contains declaration for HillsGenerator class and HillVertex struct
 * @version 0.1.0
 */

#pragma once

#include "Generator"

#include <random>

class HillsShader;

namespace HILL_DENSITY
{
  constexpr float HILLS_THIN = 3.1f * (float)WORLD_WIDTH;
  constexpr float HILLS_MEDIUM = 3.0f * (float)WORLD_WIDTH;
  constexpr float HILLS_DENSE = 2.9f * (float)WORLD_WIDTH;
}

/**
* @brief generator for hill tiles on the world map. Responsible for creating and distributing hills on the world map,
* making normal/tangent/bitangent auxiliary maps (used for normal mapping), allocating necessary data to OpenGL.
* In addition, this generator has its own buffer collection holding data after frustum culling
*/
class HillsGenerator : public Generator
{
public:
  HillsGenerator(HillsShader& shaders, const map2D_f& waterMap);
  void setup();
  void createTiles();
  void createAuxiliaryMaps();

private:
  friend class HillsRenderer;
  friend class HillsFacade;

  /**
  * @brief representation of the hill vertex as it is on the OpenGL side
  */
  struct HillVertex
  {
    constexpr static unsigned int NUMBER_OF_ELEMENTS = 14;
    HillVertex(glm::vec3 pos,
               glm::vec2 texCoords,
               glm::vec3 normal,
               glm::vec3 tangent,
               glm::vec3 bitangent) noexcept;
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
	struct
	{
		float x, y, z;;
	} tangent;
	struct
	{
		float x, y, z;
	} bitangent;
  };

  void generateMap(int cycles, float density);
  void generateKernel(int cycles, float density);
  void fattenKernel(int cycles);
  void bufferVertex(GLfloat* vertices, int offset, HillVertex vertex) noexcept;
  void fillBufferData();
  void setupVBOAttributes() noexcept;
  bool hasWaterNearby(int centerX, int centerY, int radius);
  void compressMap(float thresholdAbsValue, float ratio);
  void updateMaxHeight();
  void removePlateaus(float hubbleHeight);
  void removeOrphanHills();
  void smoothMapSinks();
  void createTangentMap();
  void createBitangentMap();

  BufferCollection culledBuffers;
  HillsShader& shaders;
  float maxHeight;
  const map2D_f& waterMap;
  map2D_vec3 normalMap;
  map2D_vec3 tangentMap;
  map2D_vec3 bitangentMap;
  std::default_random_engine randomizer;
};
