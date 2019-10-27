/*
 * Copyright 2019 Ilya Malgin
 * WaterGenerator.cpp
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
 * Purpose: contains definitions for WaterGenerator class
 * @version 0.1.0
 */

#include "WaterGenerator"
#include "WaterShader"
#include "SettingsManager"

/**
* @brief plain ctor
* @param shaders water shader manager
*/
WaterGenerator::WaterGenerator( WaterShader & shaders )
	: Generator()
	, culledBuffers( VAO | VBO | TFBO )
	, shaders( shaders )
{}

/**
* @brief prepares map
*/
void WaterGenerator::setup()
{
	generateMap();
	//if there are too little or too much water in the map - try generation again
	while( numTiles < WORLD_WIDTH * ( RIVER_WIDTH_BASE + 2 ) * ( RIVER_WIDTH_BASE + 2 ) * 9 ||
		   numTiles > WORLD_WIDTH * ( RIVER_WIDTH_BASE + 3 ) * ( RIVER_WIDTH_BASE + 3 ) * 9 )
	{
		initializeMap( map );
		generateMap();
	}
}

/**
* @brief additional post-process generation routine. Once we have shores smoothened, we should recalculate water
* in order to eliminate gaps between water and shores
* @param landMap map of the lands
*/
void WaterGenerator::setupConsiderTerrain( const map2D_f & landMap )
{
	initializeMap( postProcessMap );

	//by this moment we have smoothed shore, so make sure that water still covers it
	for( unsigned int i = 0; i < SHORE_SMOOTH_CYCLES - 1; i++ )
	{
		expandWaterArea();
	}

	//remove water data if it is under zero-level tile (which represents flat land tile)
	for( unsigned int y = 1; y < postProcessMap.size(); y++ )
	{
		for( unsigned int x = 1; x < postProcessMap[0].size(); x++ )
		{
			if( landMap[y][x] == 0 && landMap[y - 1][x] == 0 && landMap[y][x - 1] == 0 && landMap[y - 1][x - 1] == 0 )
			{
				postProcessMap[y][x] = 0;
			}
		}
	}

	createTiles();
	fillBufferData();
}

/**
* @brief creates water tiles based on generated map data
*/
void WaterGenerator::createTiles()
{
	//in case of recreation need to remove old tiles
	tiles.clear();

	for( unsigned int y = 1; y < postProcessMap.size(); y++ )
	{
		for( unsigned int x = 1; x < postProcessMap[0].size(); x++ )
		{
			if( postProcessMap[y][x] == TILE_NO_RENDER_VALUE )
			{
				continue;
			}
			if( postProcessMap[y][x] != 0 )
			{
				float lowLeft = postProcessMap[y][x];
				float lowRight = postProcessMap[y][x];
				float upRight = postProcessMap[y][x];
				float upLeft = postProcessMap[y][x];
				tiles.emplace_back( x, y, lowLeft, lowRight, upRight, upLeft );
			}
		}
	}
	tiles.shrink_to_fit();
}

/**
* @brief for each tile creates set of vertices, buffers them to GPU. Prepares buffer collections layouts and bindings
*/
void WaterGenerator::fillBufferData()
{
	numVertices = tiles.size() * UNIQUE_VERTICES_PER_TILE * WaterVertex::NUMBER_OF_ELEMENTS;
	vertices.reset( new GLfloat[numVertices] );
	const size_t INDICES_DATA_LENGTH = tiles.size() * VERTICES_PER_QUAD;
	std::unique_ptr<GLuint[]> indices( new GLuint[INDICES_DATA_LENGTH] );
	size_t indicesBufferIndex = 0;

	for( unsigned int tileIndex = 0; tileIndex < tiles.size(); tileIndex++ )
	{
		TerrainTile & tile = tiles[tileIndex];
		int x = tile.mapX, y = tile.mapY;

		//create set of vertices according to a tile
		WaterVertex lowLeft( glm::vec3( x - 1, tile.lowLeft, y ), glm::vec2( x - 1, y ) );
		WaterVertex lowRight( glm::vec3( x, tile.lowRight, y ), glm::vec2( x, y ) );
		WaterVertex upRight( glm::vec3( x, tile.upperRight, y - 1 ), glm::vec2( x, y - 1 ) );
		WaterVertex upLeft( glm::vec3( x - 1, tile.upperLeft, y - 1 ), glm::vec2( x - 1, y - 1 ) );

		//buffer vertices to local storage
		int vertexBufferOffset = tileIndex * UNIQUE_VERTICES_PER_TILE * WaterVertex::NUMBER_OF_ELEMENTS;
		bufferVertex( vertices.get(), vertexBufferOffset + WaterVertex::NUMBER_OF_ELEMENTS * 0, lowLeft );
		bufferVertex( vertices.get(), vertexBufferOffset + WaterVertex::NUMBER_OF_ELEMENTS * 1, lowRight );
		bufferVertex( vertices.get(), vertexBufferOffset + WaterVertex::NUMBER_OF_ELEMENTS * 2, upRight );
		bufferVertex( vertices.get(), vertexBufferOffset + WaterVertex::NUMBER_OF_ELEMENTS * 3, upLeft );

		//buffer indices to local storage
		GLuint indicesBufferBaseVertex = tileIndex * UNIQUE_VERTICES_PER_TILE;
		indices[indicesBufferIndex++] = indicesBufferBaseVertex + 0;
		indices[indicesBufferIndex++] = indicesBufferBaseVertex + 1;
		indices[indicesBufferIndex++] = indicesBufferBaseVertex + 2;
		indices[indicesBufferIndex++] = indicesBufferBaseVertex + 2;
		indices[indicesBufferIndex++] = indicesBufferBaseVertex + 3;
		indices[indicesBufferIndex++] = indicesBufferBaseVertex + 0;
	}
	//buffer vertices and indices from local storage to GPU
	basicGLBuffers.bind( VAO | VBO | EBO );
	glBufferData( GL_ARRAY_BUFFER, numVertices * sizeof( GLfloat ), vertices.get(), GL_STREAM_DRAW );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( GLuint ) * INDICES_DATA_LENGTH, indices.get(), GL_STATIC_DRAW );
	setupVBOAttributes();

	//prepare buffer collection used for frustum culling
	if( culledBuffers.get( VAO ) != 0 )
	{
		culledBuffers.deleteBuffers();
		culledBuffers.create( VAO | VBO | TFBO );
	}
	culledBuffers.bind( VAO | VBO | TFBO );
	/*
	 * after transform feedback we have 1.5 times more data to buffer in GL_ARRAY_BUFFER
	 * (6 indices of 4 vertices transforms into 6 vertices, 2 of them have the same attributes)
	 * because of duplication of vertices that have same attributes but different indices
	 */
	const GLsizeiptr CULLED_DATA_SIZE_BYTES = (GLsizeiptr)( (float)numVertices * 1.5f ) * sizeof( GLfloat );
	glNamedBufferStorage( culledBuffers.get( VBO ), CULLED_DATA_SIZE_BYTES, 0, GL_NONE );
	setupVBOAttributes();
	shaders.setupCulling();
	glTransformFeedbackBufferBase( culledBuffers.get( TFBO ), 0, culledBuffers.get( VBO ) );
	BufferCollection::bindZero( VAO | VBO | EBO );
}

/**
* @brief expands existing water area
*/
void WaterGenerator::expandWaterArea()
{
	//add water above the tile
	for( unsigned int y = 0; y < WORLD_HEIGHT - 1; y++ )
	{
		for( unsigned int x = 0; x < WORLD_WIDTH; x++ )
		{
			if( map[y + 1][x] != 0 )
			{
				postProcessMap[y][x] = map[y + 1][x];
			}
		}
	}
	//add more water above the tile
	for( unsigned int y = 0; y < WORLD_HEIGHT - 1; y++ )
	{
		for( unsigned int x = 0; x < WORLD_WIDTH; x++ )
		{
			if( postProcessMap[y + 1][x] != 0 )
			{
				postProcessMap[y][x] = postProcessMap[y + 1][x];
			}
		}
	}

	//add water below the tile
	for( unsigned int y = WORLD_HEIGHT; y > 0; y-- )
	{
		for( unsigned int x = 0; x < WORLD_WIDTH; x++ )
		{
			if( map[y - 1][x] != 0 )
			{
				postProcessMap[y][x] = map[y - 1][x];
			}
		}
	}
	//add more water below the tile
	for( unsigned int y = WORLD_HEIGHT; y > 0; y-- )
	{
		for( unsigned int x = 0; x < WORLD_WIDTH; x++ )
		{
			if( postProcessMap[y - 1][x] != 0 )
			{
				postProcessMap[y][x] = postProcessMap[y - 1][x];
			}
		}
	}

	//add water left to the tile
	for( unsigned int x = 0; x < WORLD_WIDTH - 1; x++ )
	{
		for( unsigned int y = 0; y < WORLD_HEIGHT; y++ )
		{
			if( map[y][x + 1] != 0 )
			{
				postProcessMap[y][x] = map[y][x + 1];
			}
		}
	}
	//add more water left to the tile
	for( unsigned int x = 0; x < WORLD_WIDTH - 1; x++ )
	{
		for( unsigned int y = 0; y < WORLD_HEIGHT; y++ )
		{
			if( postProcessMap[y][x + 1] != 0 )
			{
				postProcessMap[y][x] = postProcessMap[y][x + 1];
			}
		}
	}

	//add water right to the tile
	for( unsigned int x = WORLD_WIDTH; x > 0; x-- )
	{
		for( unsigned int y = 0; y < WORLD_HEIGHT; y++ )
		{
			if( map[y][x - 1] != 0 )
			{
				postProcessMap[y][x] = map[y][x - 1];
			}
		}
	}
	//add more water right to the tile
	for( unsigned int x = WORLD_WIDTH; x > 0; x-- )
	{
		for( unsigned int y = 0; y < WORLD_HEIGHT; y++ )
		{
			if( postProcessMap[y][x - 1] != 0 )
			{
				postProcessMap[y][x] = postProcessMap[y][x - 1];
			}
		}
	}
}

/**
* @brief generates data for water on the world map
*/
void WaterGenerator::generateMap()
{
	const float WATER_LEVEL = SettingsManager::getFloat( "SCENE", "water_level" );
	numTiles = 0;
	bool startAxisFromX = rand() % 2 == 0;
	bool riverEnd = false;
	unsigned int curveMaxDistance = rand() % RIVER_DIRECTION_CHANGE_DELAY + RIVER_DIRECTION_CHANGE_DELAY;
	unsigned int curveDistanceStep = 0;
	int y, x;
	unsigned int startCoord = rand() % WORLD_HEIGHT;
	x = startAxisFromX ? startCoord : 0;
	y = startAxisFromX ? 0 : startCoord;
	DIRECTION riverDirection = startAxisFromX ? DOWN : RIGHT;
	int riverWidthOffset = 0, kernelCounter = 0;
	bool riverWidthIncrease = true;

	while( !riverEnd )
	{
		switch( riverDirection )
		{
		case UP:
		{
			++curveDistanceStep;
			--y;
			if( y <= 0 )
			{
				y = 0;
				riverEnd = true;
			}
			if( rand() % 4 == 0 )
			{
				x += rand() % 2 == 0 ? 2 : -2;
			}
			if( x <= 0 )
			{
				x = 0;
				riverEnd = true;
			}
			if( x >= WORLD_WIDTH )
			{
				x = WORLD_WIDTH;
				riverEnd = true;
			}
			map[y][x] = WATER_LEVEL;
			++numTiles;
			if( curveDistanceStep == curveMaxDistance )
			{
				setNewDirection( curveDistanceStep, curveMaxDistance, riverDirection, UP_LEFT, UP_RIGHT );
			}
			break;
		}
		case UP_RIGHT:
		{
			++curveDistanceStep;
			y -= rand() % 2;
			x += rand() % 2;
			if( y <= 0 )
			{
				y = 0;
				riverEnd = true;
			}
			if( x >= WORLD_WIDTH )
			{
				x = WORLD_WIDTH;
				riverEnd = true;
			}
			map[y][x] = WATER_LEVEL;
			++numTiles;
			if( curveDistanceStep == curveMaxDistance )
			{
				setNewDirection( curveDistanceStep, curveMaxDistance, riverDirection, UP, RIGHT );
			}
			break;
		}
		case RIGHT:
		{
			++curveDistanceStep;
			++x;
			if( x >= WORLD_WIDTH )
			{
				x = WORLD_WIDTH;
				riverEnd = true;
			}
			if( rand() % 4 == 0 )
			{
				y += rand() % 2 == 0 ? 2 : -2;
			}
			if( y <= 0 )
			{
				y = 0;
				riverEnd = true;
			}
			if( y >= WORLD_HEIGHT )
			{
				y = WORLD_HEIGHT;
				riverEnd = true;
			}
			map[y][x] = WATER_LEVEL;
			++numTiles;
			if( curveDistanceStep == curveMaxDistance )
			{
				setNewDirection( curveDistanceStep, curveMaxDistance, riverDirection, UP_RIGHT, DOWN_RIGHT );
			}
			break;
		}
		case DOWN_RIGHT:
		{
			++curveDistanceStep;
			y += rand() % 2;
			x += rand() % 2;
			if( y >= WORLD_HEIGHT )
			{
				y = WORLD_HEIGHT;
				riverEnd = true;
			}
			if( x >= WORLD_WIDTH )
			{
				x = WORLD_WIDTH;
				riverEnd = true;
			}
			map[y][x] = WATER_LEVEL;
			++numTiles;
			if( curveDistanceStep == curveMaxDistance )
			{
				setNewDirection( curveDistanceStep, curveMaxDistance, riverDirection, DOWN, RIGHT );
			}
			break;
		}
		case DOWN:
		{
			++curveDistanceStep;
			++y;
			if( y >= WORLD_HEIGHT )
			{
				y = WORLD_HEIGHT;
				riverEnd = true;
			}
			if( rand() % 4 == 0 )
			{
				x += rand() % 2 == 0 ? 2 : -2;
			}
			if( x <= 0 )
			{
				x = 0;
				riverEnd = true;
			}
			if( x >= WORLD_WIDTH )
			{
				x = WORLD_WIDTH;
				riverEnd = true;
			}
			map[y][x] = WATER_LEVEL;
			++numTiles;
			if( curveDistanceStep == curveMaxDistance )
			{
				setNewDirection( curveDistanceStep, curveMaxDistance, riverDirection, DOWN_RIGHT, DOWN_LEFT );
			}
			break;
		}
		case DOWN_LEFT:
		{
			++curveDistanceStep;
			y += rand() % 2;
			x -= rand() % 2;
			if( y >= WORLD_HEIGHT )
			{
				y = WORLD_HEIGHT;
				riverEnd = true;
			}
			if( x <= 0 )
			{
				x = 0;
				riverEnd = true;
			}
			map[y][x] = WATER_LEVEL;
			++numTiles;
			if( curveDistanceStep == curveMaxDistance )
			{
				setNewDirection( curveDistanceStep, curveMaxDistance, riverDirection, DOWN, LEFT );
			}
			break;
		}
		case LEFT:
		{
			++curveDistanceStep;
			--x;
			if( x <= 0 )
			{
				x = 0;
				riverEnd = true;
			}
			if( rand() % 4 == 0 )
			{
				y += rand() % 2 == 0 ? 2 : -2;
			}
			if( y <= 0 )
			{
				y = 0;
				riverEnd = true;
			}
			if( y >= WORLD_HEIGHT )
			{
				y = WORLD_HEIGHT;
				riverEnd = true;
			}
			map[y][x] = WATER_LEVEL;
			++numTiles;
			if( curveDistanceStep == curveMaxDistance )
			{
				setNewDirection( curveDistanceStep, curveMaxDistance, riverDirection, UP_LEFT, DOWN_LEFT );
			}
			break;
		}
		case UP_LEFT:
		{
			++curveDistanceStep;
			y -= rand() % 2;
			x -= rand() % 2;
			if( x <= 0 )
			{
				x = 0;
				riverEnd = true;
			}
			if( y <= 0 )
			{
				y = 0;
				riverEnd = true;
			}
			map[y][x] = WATER_LEVEL;
			++numTiles;
			if( curveDistanceStep == curveMaxDistance )
			{
				setNewDirection( curveDistanceStep, curveMaxDistance, riverDirection, UP, LEFT );
			}
			break;
		}
		default:
			break;
		}
		fattenKernel( x, y, kernelCounter, riverWidthOffset, riverWidthIncrease );
	}
}

/**
* @brief sets new incremental generating direction
* @param curveDistanceStep incremental step of previous direction of generating
* @param curveMaxDistance maximum steps for new direction during generating process
* @param currentDirection current direction of generating
* @param validDirectionLeft valid expected new direction if it curves left
* @param validDirectionRight valid expected new direction if it curves right
*/
void WaterGenerator::setNewDirection( unsigned int & curveDistanceStep,
									  unsigned int & curveMaxDistance,
									  DIRECTION & currentDirection,
									  DIRECTION validDirectionLeft,
									  DIRECTION validDirectionRight ) noexcept
{
	curveDistanceStep = 0;
	curveMaxDistance = rand() % RIVER_DIRECTION_CHANGE_DELAY + RIVER_DIRECTION_CHANGE_DELAY;
	currentDirection = (DIRECTION)( rand() % DIRECTION::NUM_DIRECTIONS );
	while( currentDirection != validDirectionLeft && currentDirection != validDirectionRight )
	{
		currentDirection = (DIRECTION)( rand() % DIRECTION::NUM_DIRECTIONS );
	}
}

/**
* @brief fattens generated water kernel value
* @param x x coordinate of water kernel
* @param y y coordinate of water kernel
* @param kernelCounter virtual counter defining when frequency of river width updating
* @param riverWidthOffset additional offset to width of the river
* @param riverWidthIncrease defines whether width offset is currently incrementing
*/
void WaterGenerator::fattenKernel( int x, 
								   int y, 
								   int & kernelCounter, 
								   int & riverWidthOffset, 
								   bool & riverWidthIncrease )
{
	//calculate area coordinates to add water to
	int shoreSizeYT = rand() % 2 + RIVER_WIDTH_BASE;
	int shoreSizeYB = rand() % 2 + RIVER_WIDTH_BASE;
	int shoreSizeXL = rand() % 2 + RIVER_WIDTH_BASE;
	int shoreSizeXR = rand() % 2 + RIVER_WIDTH_BASE;

	//check if we need to update width offset 
	const unsigned int RIVER_SIZE_TO_INCREASE_COUNTER = 19;
	++kernelCounter;
	if( kernelCounter % RIVER_SIZE_TO_INCREASE_COUNTER == 0 )
	{
		kernelCounter = 0;
		riverWidthOffset += riverWidthIncrease ? 1 : -1;
		if( riverWidthOffset > RIVER_WIDTH_BASE )
		{
			riverWidthIncrease = !riverWidthIncrease;
		}
		else if( riverWidthOffset < 0 )
		{
			riverWidthOffset = 0;
			riverWidthIncrease = !riverWidthIncrease;
		}
	}

	//sanitize area coordinates and apply offset
	int xLeft = ( (int)( x - shoreSizeXL - riverWidthOffset ) <= 0 ? 0 : x - shoreSizeXL - riverWidthOffset );
	int xRight = ( (int)( x + shoreSizeXR + riverWidthOffset ) >= WORLD_WIDTH ? WORLD_WIDTH : x + shoreSizeXR + riverWidthOffset );
	int yTop = ( (int)( y - shoreSizeYT - riverWidthOffset ) <= 0 ? 0 : y - shoreSizeYT - riverWidthOffset );
	int yBottom = ( (int)( y + shoreSizeYB + riverWidthOffset ) >= WORLD_HEIGHT ? WORLD_HEIGHT : y + shoreSizeYB + riverWidthOffset );

	//pour the water around
	const float WATER_LEVEL = SettingsManager::getFloat( "SCENE", "water_level" );
	for( int y1 = yTop; y1 <= yBottom; y1++ )
	{
		for( int x1 = xLeft; x1 <= xRight; x1++ )
		{
			map[y1][x1] = WATER_LEVEL;
			++numTiles;
		}
	}
}

/**
* @brief helper function that buffers a vertex to local storage
* @param vertices local buffer
* @param offset index offset of buffer
* @param vertex water vertex to be buffered
*/
void WaterGenerator::bufferVertex( GLfloat * vertices, 
								   int offset, 
								   WaterVertex vertex ) noexcept
{
	vertices[offset + 0] = vertex.position.x;
	vertices[offset + 1] = vertex.position.y;
	vertices[offset + 2] = vertex.position.z;
	vertices[offset + 3] = vertex.animationOffset;
}

/**
* @brief helper function that sets appropriate layout for vertex data on the OpenGL side
*/
void WaterGenerator::setupVBOAttributes() noexcept
{
	const size_t SIZE_OF_WATER_VERTEX = WaterVertex::NUMBER_OF_ELEMENTS * sizeof( GLfloat );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, WaterVertex::NUMBER_OF_ELEMENTS, GL_FLOAT, GL_FALSE, SIZE_OF_WATER_VERTEX, 0 );
}

/**
* @brief plain ctor
* @param position water vertex XYZ position
* @param animOffsetVec pair of XZ position dependent values crunched into an offset value for water animation
*/
WaterGenerator::WaterVertex::WaterVertex( glm::vec3 position, 
										  glm::vec2 animOffsetVec ) noexcept
	: position{ position.x - HALF_WORLD_WIDTH, position.y, position.z - HALF_WORLD_HEIGHT }
	, animationOffset( animOffsetVec.x * X_POS_ANIM_MULTIPLIER + animOffsetVec.y * ( animOffsetVec.x + X_POS_ANIM_OFFSET ) )
{}
