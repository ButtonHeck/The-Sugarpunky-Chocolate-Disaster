/*
 * Copyright 2019 Ilya Malgin
 * LandGenerator.cpp
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
 * Purpose: contains definitions for LandGenerator class
 * @version 0.1.0
 */

#include "LandGenerator"

#include <chrono>

/**
* @brief plain ctor. Explicitly initializes cells buffer collection for indirect buffer usage, initializes randomizer seed
*/
LandGenerator::LandGenerator() noexcept
	: Generator()
	, cellBuffers( VAO | VBO | INSTANCE_VBO | EBO | DIBO )
{
	randomizer.seed( std::chrono::system_clock::now().time_since_epoch().count() );
	//this collection already have VAO/VBO/EBO in Generator ctor
	basicGLBuffers.add( INSTANCE_VBO );
}

/**
* @brief prepares land map and buffer collections
* @param shoreMap map of the shore tiles
*/
void LandGenerator::setup( const map2D_f & shoreMap )
{
	initializeMap( chunkMap );
	//for land generator just copy the shore map
	map = shoreMap;
	splitChunks( CHUNK_SIZE );
	tiles.shrink_to_fit();
	splitCellChunks( CHUNK_SIZE );
	fillBufferData();
	fillCellBufferData();
}

/**
* @brief setup square chunks storage
* @param chunkSize size of one square chunk
*/
void LandGenerator::splitChunks( int chunkSize )
{
	//in case of recreation need to remove old data
	chunks.clear();
	tiles.clear();
	unsigned int chunkOffset = 0;
	for( int startY = 0; startY < WORLD_HEIGHT - chunkSize + 1; startY += chunkSize )
	{
		for( int startX = 0; startX < WORLD_WIDTH - chunkSize + 1; startX += chunkSize )
		{
			//check if region of the map is flat
			bool chunkIsFlat = true;
			for( int y = startY; y < startY + chunkSize; y++ )
			{
				for( int x = startX; x < startX + chunkSize; x++ )
				{
					if( map[y][x] != 0 || map[y + 1][x] != 0 || map[y + 1][x + 1] != 0 || map[y][x + 1] != 0 )
					{
						chunkIsFlat = false;
						break;
					}
				}
				if( !chunkIsFlat )
				{
					break;
				}
			}
			if( chunkIsFlat )
			{
				for( int noRenderY = startY + 1; noRenderY < startY + chunkSize; noRenderY++ )
				{
					for( int noRenderX = startX + 1; noRenderX < startX + chunkSize; noRenderX++ )
					{
						map[noRenderY][noRenderX] = 0;
						chunkMap[noRenderY][noRenderX] = CHUNK_NO_RENDER_VALUE;
					}
				}
				//for square map segments one tile is also one chunk
				tiles.emplace_back( startX, startY, 0.0f, 0.0f, 0.0f, 0.0f );
				chunks.emplace_back( startX, startX + chunkSize, startY, startY + chunkSize, chunkOffset, 1 );
				++chunkOffset;
			}
		}
	}
}

/**
* @brief setup cells chunks storage
* @param chunkSize size of one cells chunk
*/
void LandGenerator::splitCellChunks( int chunkSize )
{
	//in case of recreation need to remove old data
	cellTiles.clear();
	cellChunks.clear();
	unsigned int chunkOffset = 0;
	for( int startY = 0; startY < WORLD_HEIGHT - chunkSize + 1; startY += chunkSize )
	{
		for( int startX = 0; startX < WORLD_WIDTH - chunkSize + 1; startX += chunkSize )
		{
			unsigned int cellInstances = 0;
			for( int y = startY + 1; y < startY + chunkSize + 1; y++ )
			{
				for( int x = startX; x < startX + chunkSize; x++ )
				{
					//check if this segment is flat and has not been used in square chunk storage
					if( map[y][x] == 0 && map[y - 1][x] == 0 && map[y - 1][x + 1] == 0 && map[y][x + 1] == 0 &&
						chunkMap[y][x] != CHUNK_NO_RENDER_VALUE &&
						chunkMap[y - 1][x] != CHUNK_NO_RENDER_VALUE &&
						chunkMap[y - 1][x + 1] != CHUNK_NO_RENDER_VALUE &&
						chunkMap[y][x + 1] != CHUNK_NO_RENDER_VALUE )
					{
						cellTiles.emplace_back( x, y, 0.0f, 0.0f, 0.0f, 0.0f );
						cellInstances++;
					}
				}
			}
			if( cellInstances != 0 )
			{
				cellChunks.emplace_back( startX, startX + chunkSize, startY, startY + chunkSize, chunkOffset, cellInstances );
			}
			chunkOffset += cellInstances;
		}
	}
}

/**
* @brief prepares square segments buffer collection
*/
void LandGenerator::fillBufferData()
{
	//buffer data for exactly one square chunk
	float halfChunkSize = CHUNK_SIZE / 2;
	const unsigned int CHUNK_VERTICES_SIZE_FLOATS = 20;
	GLfloat chunkVertices[CHUNK_VERTICES_SIZE_FLOATS] = {
		-halfChunkSize, 0.0f,  halfChunkSize, 0.0f,               0.0f,
		 halfChunkSize, 0.0f,  halfChunkSize, (float)CHUNK_SIZE,  0.0f,
		 halfChunkSize, 0.0f, -halfChunkSize, (float)CHUNK_SIZE,  (float)CHUNK_SIZE,
		-halfChunkSize, 0.0f, -halfChunkSize, 0.0f,               (float)CHUNK_SIZE
	};
	basicGLBuffers.bind( VAO );
	bufferData( basicGLBuffers, chunkVertices, CHUNK_VERTICES_SIZE_FLOATS );
	setupVBOAttributes();

	//buffer instances translations data to instance vbo
	std::unique_ptr<glm::vec4[]> landChunkInstanceTranslations( new glm::vec4[tiles.size()] );
	for( unsigned int tileIndex = 0; tileIndex < tiles.size(); tileIndex++ )
	{
		TerrainTile & tile = tiles[tileIndex];
		landChunkInstanceTranslations[tileIndex] = glm::vec4( -HALF_WORLD_WIDTH + tile.mapX + halfChunkSize,
															  0.0f,
															  -HALF_WORLD_HEIGHT + tile.mapY + halfChunkSize,
															  0.0f );
	}
	basicGLBuffers.bind( INSTANCE_VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec4 ) * tiles.size(), &landChunkInstanceTranslations[0], GL_STATIC_DRAW );
	setupVBOInstancedAttributes();
	BufferCollection::bindZero( VAO | VBO | EBO );
}

/**
* @brief prepares cells chunks buffer collection
*/
void LandGenerator::fillCellBufferData()
{
	//buffer data for exactly one cell
	const unsigned int CELL_VERTICES_SIZE_FLOATS = 20;
	GLfloat cellVertices[CELL_VERTICES_SIZE_FLOATS] = {
		 0.0f, 0.0f,  1.0f, 0.0f,  0.0f,
		 1.0f, 0.0f,  1.0f, 1.0f,  0.0f,
		 1.0f, 0.0f,  0.0f, 1.0f,  1.0f,
		 0.0f, 0.0f,  0.0f, 0.0f,  1.0f
	};
	cellBuffers.bind( VAO | DIBO );
	bufferData( cellBuffers, cellVertices, CELL_VERTICES_SIZE_FLOATS );
	setupVBOAttributes();

	//buffer cells instances data to instance vbo
	std::unique_ptr<glm::vec4[]> cellInstanceTranslations( new glm::vec4[cellTiles.size()] );
	for( unsigned int tileIndex = 0; tileIndex < cellTiles.size(); tileIndex++ )
	{
		TerrainTile & tile = cellTiles[tileIndex];
		cellInstanceTranslations[tileIndex] = glm::vec4( -HALF_WORLD_WIDTH + tile.mapX, 0.0f, -HALF_WORLD_HEIGHT + tile.mapY - 1, 0.0f );
	}
	cellBuffers.bind( INSTANCE_VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec4 ) * cellTiles.size(), &cellInstanceTranslations[0], GL_STATIC_DRAW );
	setupVBOInstancedAttributes();
	BufferCollection::bindZero( VAO | VBO | EBO );
}

/**
* @brief helper function to load data from given buffer to buffer collection
* @param bufferCollection buffer collection to load data to
* @param buffer client buffer storage
* @param size number of elements to load
*/
void LandGenerator::bufferData( BufferCollection & bufferCollection, 
								GLfloat * buffer, 
								size_t size )
{
	bufferCollection.bind( VBO | EBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( QUAD_INDICES ), QUAD_INDICES, GL_STATIC_DRAW );
	glBufferData( GL_ARRAY_BUFFER, sizeof( buffer ) * size, buffer, GL_STATIC_DRAW );
}

/**
* @brief helper function to setup vbo attributes and pointers
*/
void LandGenerator::setupVBOAttributes() noexcept
{
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), 0 );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), (void*)( 3 * sizeof( GLfloat ) ) );
}

/**
* @brief helper function to setup instance vbo attribute, pointer and divisor
*/
void LandGenerator::setupVBOInstancedAttributes() noexcept
{
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof( glm::vec4 ), 0 );
	glVertexAttribDivisor( 2, 1 );
}

/**
* @brief updates indirect buffer data for cells storage
* @param frustum camera's view frustum
*/
void LandGenerator::updateCellsIndirectBuffer( const Frustum & frustum )
{
	cellBuffers.bind( VAO );
	// {indicesCount, numInstancesToDraw, firstIndex, baseVertex, baseInstance}
	std::unique_ptr<GLuint[]> indirectBuffer( new GLuint[cellChunks.size() * INDIRECT_DRAW_COMMAND_ARGUMENTS] );
	GLuint dataOffset = 0;
	cellPrimitiveCount = 0;
	for( unsigned int chunkIndex = 0; chunkIndex < cellChunks.size(); chunkIndex++ )
	{
		if( cellChunks[chunkIndex].isInsideFrustum( frustum, 1.0f ) )
		{
			++cellPrimitiveCount;
			addIndirectBufferData( &indirectBuffer[0], dataOffset, cellChunks[chunkIndex].getNumInstances(), cellChunks[chunkIndex].getInstanceOffset() );
		}
	}
	cellBuffers.bind( DIBO );
	glBufferData( GL_DRAW_INDIRECT_BUFFER, INDIRECT_DRAW_COMMAND_BYTE_SIZE * cellPrimitiveCount, &indirectBuffer[0], GL_STATIC_DRAW );
}

/**
* @brief load data of one inderect draw command to indirect buffer
* @param buffer client buffer storage
* @param dataOffset offset to storage index
* @param numInstances number of instances for draw call
* @param instanceOffset instance offset for draw call
*/
void LandGenerator::addIndirectBufferData( GLuint * buffer, 
										   GLuint & dataOffset, 
										   GLuint numInstances, 
										   GLuint instanceOffset ) noexcept
{
	buffer[dataOffset++] = VERTICES_PER_QUAD;
	buffer[dataOffset++] = numInstances;
	buffer[dataOffset++] = 0;
	buffer[dataOffset++] = 0;
	buffer[dataOffset++] = instanceOffset;
}
