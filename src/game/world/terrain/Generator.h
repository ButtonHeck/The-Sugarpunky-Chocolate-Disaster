/*
 * Copyright 2019 Ilya Malgin
 * Generator.h
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
 * Purpose: contains declaration for Generator class
 * @version 0.1.0
 */

#pragma once

#include "TerrainTile"
#include "SceneSettings"
#include "TypeAliases"
#include "BufferCollection"

#include <vector>
#include <fstream>

constexpr unsigned int UNIQUE_VERTICES_PER_TILE = 4;

/**
* @brief base class for generators. Each generator contains a map representing distribution of different kind of terrain on it.
* Each generator has its own storage of terrain tiles and a GL buffer collection (some subclasses may have additional ones).
* Responsible for map initialization, buffer collection initialization, (de-)serialization.
* Has some utility functions to transform map data
*/
class Generator
{
public:
	Generator() noexcept;
	virtual ~Generator() = default;
	const map2D_f & getMap() const noexcept;
	virtual void serialize( std::ofstream & output, 
							bool usePrecision = false, 
							unsigned int precision = 6 );
	virtual void deserialize( std::ifstream & input );

	/**
	* @brief creates storage for map data and initializes it with zeroes
	* @param map 2D map to initialize
	* @note made static because there might be use cases when non-member map are in use (e.g. distribution map for plants or normal map)
	*/
	template <typename T>
	static void initializeMap( map2D_template<T> & map )
	{
		map.clear();
		map.reserve( WORLD_HEIGHT + 1 );
		for( size_t row = 0; row < WORLD_HEIGHT + 1; row++ )
		{
			map.emplace_back( std::vector<T>( WORLD_WIDTH + 1, 0 ) );
		}
	}
	void smoothMapAdjacentHeights( float selfWeight, 
								   float sideNeighbourWeight, 
								   float diagonalNeighbourWeight );
	void createNormalMap( map2D_vec3 & normalMap );

protected:
	map2D_f map;
	std::vector<TerrainTile> tiles;
	BufferCollection basicGLBuffers;

private:
	template <typename T>
	void serializeRepeatValues( std::ofstream & output,
								T value,
								const unsigned int & row,
								unsigned int & column );
	template <typename T>
	void deserializeRepeatValues( std::ifstream & input,
								  T value,
								  const unsigned int & row,
								  unsigned int & column );
};
