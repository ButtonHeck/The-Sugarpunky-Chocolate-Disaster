/*
 * Copyright 2019 Ilya Malgin
 * ShoreGenerator.cpp
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
 * Purpose: contains definitions for ShoreGenerator class and ShoreVertex struct
 * @version 0.1.0
 */

#include "ShoreGenerator"
#include "SettingsManager"

#include <chrono>
#include <memory>

/**
* @brief plain ctor
* @param waterMap map of the water
*/
ShoreGenerator::ShoreGenerator( const map2D_f & waterMap )
	: Generator()
	, waterMap( waterMap )
{
	randomizer.seed( std::chrono::system_clock::now().time_since_epoch().count() );
}

/**
* @brief prepares shore map and buffer collections
*/
void ShoreGenerator::setup()
{
	generateMap();
	for( unsigned int cycleCount = 0; cycleCount < SHORE_SMOOTH_CYCLES; cycleCount++ )
	{
		shapeShoreProfile();
		float selfWeight = 0.5f - 0.02f * cycleCount;
		float sideNeighbourWeight = ( 1.0f - selfWeight ) / 8.0f;
		float diagonalNeighbourWeight = sideNeighbourWeight;
		smoothMapAdjacentHeights( selfWeight, sideNeighbourWeight, diagonalNeighbourWeight );
	}
	randomizeShore();
	applySlopeToProfile( 2.0f );
	correctMapAtEdges();
	const float SHORE_CUT_LEVEL = -4.0f;
	removeUnderwaterTiles( SHORE_CUT_LEVEL );
	createTiles();
	createNormalMap( normalMap );
	fillBufferData();
}

/**
* @brief generates shore data based on existing water map data
* @todo remove shore generation artefacts on the edges of world map
*/
void ShoreGenerator::generateMap()
{
	const float MIN_HEIGHT_KERNEL_OFFSET = 0.9f;
	const float MAX_HEIGHT_KERNEL_OFFSET = 1.1f;
	std::uniform_real_distribution<float> positionDistribution( MIN_HEIGHT_KERNEL_OFFSET, MAX_HEIGHT_KERNEL_OFFSET );

	for( unsigned int y = 0; y <= WORLD_HEIGHT; y++ )
	{
		for( unsigned int x = 0; x <= WORLD_WIDTH; x++ )
		{
			map[y][x] = waterMap[y][x] * 1.1f * positionDistribution( randomizer );
		}
	}
}

/**
* @brief makes shore looks like a shore - farther tiles from land are deeper under water
*/
void ShoreGenerator::shapeShoreProfile()
{
	const float HEIGHT_SMOOTH_OFFSET = 0.25f;
	float waterLevel = SettingsManager::getFloat( "SCENE", "water_level" ) + HEIGHT_SMOOTH_OFFSET;

	//smooth tile below on map
	for( unsigned int y = 1; y < WORLD_HEIGHT + 1; y++ )
	{
		for( unsigned int x = 0; x < WORLD_WIDTH + 1; x++ )
		{
			if( map[y - 1][x] < waterLevel - waterLevel * 0.25f )
			{
				map[y][x] += waterLevel * 0.5f;
			}
		}
	}
	//smooth tile upper on map
	for( unsigned int y = 0; y < WORLD_HEIGHT; y++ )
	{
		for( unsigned int x = 0; x < WORLD_WIDTH + 1; x++ )
		{
			if( map[y + 1][x] < waterLevel - waterLevel * 0.25f )
			{
				map[y][x] += waterLevel * 0.5f;
			}
		}
	}
	//smooth tile left on map
	for( unsigned int y = 0; y < WORLD_HEIGHT + 1; y++ )
	{
		for( unsigned int x = 0; x < WORLD_WIDTH; x++ )
		{
			if( map[y][x + 1] < waterLevel - waterLevel * 0.25f )
			{
				map[y][x] += waterLevel * 0.5f;
			}
		}
	}
	//smooth tile right on map
	for( unsigned int y = 0; y < WORLD_HEIGHT + 1; y++ )
	{
		for( unsigned int x = 1; x < WORLD_WIDTH + 1; x++ )
		{
			if( map[y][x - 1] < waterLevel - waterLevel * 0.25f )
			{
				map[y][x] += waterLevel * 0.5f;
			}
		}
	}
}

/**
* @brief randomizes shore profile a bit
*/
void ShoreGenerator::randomizeShore()
{
	const float MIN_HEIGHT_RANDOMIZE_OFFSET = -0.24f;
	const float MAX_HEIGHT_RANDOMIZE_OFFSET = 0.24f;
	std::uniform_real_distribution<float> distribution( MIN_HEIGHT_RANDOMIZE_OFFSET, MAX_HEIGHT_RANDOMIZE_OFFSET );

	for( unsigned int y = 0; y < WORLD_HEIGHT; y++ )
	{
		for( unsigned int x = 0; x < WORLD_WIDTH; x++ )
		{
			if( map[y][x] < 0 )
			{
				map[y][x] += distribution( randomizer );
			}
		}
	}
}

/**
* @brief removes shore tiles at map edges if there is water
*/
void ShoreGenerator::correctMapAtEdges()
{
	//correct top and bottom sides of the map
	for( unsigned int x = 0; x < WORLD_WIDTH; ++x )
	{
		if( waterMap[0][x] != 0 )
		{
			map[0][x] = map[1][x];
		}
		if( waterMap[WORLD_HEIGHT][x] != 0 )
		{
			map[WORLD_HEIGHT][x] = map[WORLD_HEIGHT - 1][x];
		}
	}
	//correct left and right sides of the map
	for( unsigned int y = 0; y < WORLD_HEIGHT; ++y )
	{
		if( waterMap[y][0] != 0 )
		{
			map[y][0] = map[y][1];
		}
		if( waterMap[y][WORLD_WIDTH] != 0 )
		{
			map[y][WORLD_WIDTH] = map[y][WORLD_WIDTH - 1];
		}
	}
}

/**
* @brief makes shore profile less steep for a given ratio
* @param ratio slope ratio, higher number means more gentle profile sloping
* @note think of this function as an inverse of compression - farther from zero level values lifted up for a bigger amount
*/
void ShoreGenerator::applySlopeToProfile( float ratio ) noexcept
{
	for( std::vector<float> & row : map )
	{
		for( float & height : row )
		{
			if( height < 0 )
			{
				height /= ratio;
			}
		}
	}
}

/**
* @brief marks all map points that are below underwater level as non-tileable
* @param thresholdValue value below which a map coordinate would be treated as non-tileable
*/
void ShoreGenerator::removeUnderwaterTiles( float thresholdValue )
{
	for( unsigned int y = 1; y < WORLD_HEIGHT; y++ )
	{
		/*
		* check neighbours also to ensure that there would be no visual gaps between underwater and shore
		* if we would not create a tile using this coordinate
		*/
		for( unsigned int x = 1; x < WORLD_WIDTH; x++ )
		{
			if( map[y - 1][x - 1] < thresholdValue &&
				map[y - 1][x] < thresholdValue &&
				map[y - 1][x + 1] < thresholdValue &&
				map[y][x - 1] < thresholdValue &&
				map[y][x] < thresholdValue &&
				map[y][x + 1] < thresholdValue &&
				map[y + 1][x - 1] < thresholdValue &&
				map[y + 1][x] < thresholdValue &&
				map[y + 1][x + 1] < thresholdValue )
			{
				map[y][x] = TILE_NO_RENDER_VALUE;
			}
		}
	}
}

/**
* @brief creates tiles based on processed map data
*/
void ShoreGenerator::createTiles()
{
	//in case of recreation need to remove old tiles
	tiles.clear();

	for( unsigned int y = 1; y < map.size(); y++ )
	{
		for( unsigned int x = 1; x < map[0].size(); x++ )
		{
			if( map[y][x] == TILE_NO_RENDER_VALUE )
			{
				continue;
			}
			if( map[y][x] != 0 || map[y - 1][x] != 0 || map[y][x - 1] != 0 || map[y - 1][x - 1] != 0 )
			{
				float lowLeft = ( map[y][x - 1] == TILE_NO_RENDER_VALUE ? map[y][x] : map[y][x - 1] );
				float lowRight = map[y][x];
				float upRight = ( map[y - 1][x] == TILE_NO_RENDER_VALUE ? map[y][x] : map[y - 1][x] );
				float upLeft = ( map[y - 1][x - 1] == TILE_NO_RENDER_VALUE ? map[y][x] : map[y - 1][x - 1] );
				tiles.emplace_back( x, y, lowLeft, lowRight, upRight, upLeft );
			}
		}
	}
	tiles.shrink_to_fit();
}

/**
* @brief prepares buffer collection
*/
void ShoreGenerator::fillBufferData()
{
	using glm::vec2;
	using glm::vec3;
	const size_t VERTEX_DATA_LENGTH = tiles.size() * UNIQUE_VERTICES_PER_TILE * ShoreVertex::NUMBER_OF_ELEMENTS;
	const size_t INDICES_DATA_LENGTH = tiles.size() * VERTICES_PER_QUAD;
	size_t indicesBufferIndex = 0;
	std::unique_ptr<GLfloat[]> vertices( new GLfloat[VERTEX_DATA_LENGTH] );
	std::unique_ptr<GLuint[]> indices( new GLuint[INDICES_DATA_LENGTH] );
	for( unsigned int tileIndex = 0; tileIndex < tiles.size(); tileIndex++ )
	{
		TerrainTile & tile = tiles[tileIndex];
		int x = tile.mapX, y = tile.mapY;

		ShoreVertex lowLeft( vec3( x - 1, tile.lowLeft, y ), vec2( 0.0f ), normalMap[y][x - 1] );
		ShoreVertex lowRight( vec3( x, tile.lowRight, y ), vec2( 1.0f, 0.0f ), normalMap[y][x] );
		ShoreVertex upRight( vec3( x, tile.upperRight, y - 1 ), vec2( 1.0f ), normalMap[y - 1][x] );
		ShoreVertex upLeft( vec3( x - 1, tile.upperLeft, y - 1 ), vec2( 0.0f, 1.0f ), normalMap[y - 1][x - 1] );

		int vertexBufferOffset = tileIndex * UNIQUE_VERTICES_PER_TILE * ShoreVertex::NUMBER_OF_ELEMENTS;
		bufferVertex( vertices.get(), vertexBufferOffset + ShoreVertex::NUMBER_OF_ELEMENTS * 0, lowLeft );
		bufferVertex( vertices.get(), vertexBufferOffset + ShoreVertex::NUMBER_OF_ELEMENTS * 1, lowRight );
		bufferVertex( vertices.get(), vertexBufferOffset + ShoreVertex::NUMBER_OF_ELEMENTS * 2, upRight );
		bufferVertex( vertices.get(), vertexBufferOffset + ShoreVertex::NUMBER_OF_ELEMENTS * 3, upLeft );

		GLuint indicesBufferBaseVertex = tileIndex * UNIQUE_VERTICES_PER_TILE;
		indices[indicesBufferIndex++] = indicesBufferBaseVertex + 0;
		indices[indicesBufferIndex++] = indicesBufferBaseVertex + 1;
		indices[indicesBufferIndex++] = indicesBufferBaseVertex + 2;
		indices[indicesBufferIndex++] = indicesBufferBaseVertex + 2;
		indices[indicesBufferIndex++] = indicesBufferBaseVertex + 3;
		indices[indicesBufferIndex++] = indicesBufferBaseVertex + 0;
	}

	basicGLBuffers.bind( VAO | VBO | EBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * VERTEX_DATA_LENGTH, vertices.get(), GL_STATIC_DRAW );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( GLuint ) * INDICES_DATA_LENGTH, indices.get(), GL_STATIC_DRAW );

	const size_t SIZE_OF_SHORE_VERTEX = ShoreVertex::NUMBER_OF_ELEMENTS * sizeof( GLfloat );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, SIZE_OF_SHORE_VERTEX, 0 );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, SIZE_OF_SHORE_VERTEX, (void*)( 3 * sizeof( GLfloat ) ) );
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, SIZE_OF_SHORE_VERTEX, (void*)( 5 * sizeof( GLfloat ) ) );
	BufferCollection::bindZero( VAO | VBO | EBO );
}

/**
* @brief helper function that fills data of a shore vertex to local storage
* @param vertices vertices local storage
* @param offset index offset to storage
* @param vertex shore vertex to be bufferred
*/
void ShoreGenerator::bufferVertex( GLfloat * vertices, 
								   int offset, 
								   ShoreVertex vertex ) noexcept
{
	vertices[offset + 0] = vertex.position.x;
	vertices[offset + 1] = vertex.position.y;
	vertices[offset + 2] = vertex.position.z;
	vertices[offset + 3] = vertex.texCoords.x;
	vertices[offset + 4] = vertex.texCoords.y;
	vertices[offset + 5] = vertex.normal.x;
	vertices[offset + 6] = vertex.normal.y;
	vertices[offset + 7] = vertex.normal.z;
}

/**
* @brief plain ctor
*/
ShoreGenerator::ShoreVertex::ShoreVertex( glm::vec3 position, 
										  glm::vec2 texCoords, 
										  glm::vec3 normal ) noexcept
	: position{ position.x - HALF_WORLD_WIDTH, position.y, position.z - HALF_WORLD_HEIGHT }
	, texCoords{ texCoords.x, texCoords.y }
	, normal{ normal.x, normal.y, normal.z }
{}
