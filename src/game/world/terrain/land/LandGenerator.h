/*
 * Copyright 2019 Ilya Malgin
 * LandGenerator.h
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
 * Purpose: contains declaration for LandGenerator class
 * @version 0.1.0
 */

#pragma once

#include "Generator"
#include "LandChunk"

#include <random>

/**
* @brief Generator for land terrain data. Has two types of storages: square tile chunks (that are not cut off by shore)
* and cells chunks (non-square regions nearby shore).
*/
class LandGenerator : public Generator
{
public:
	LandGenerator() noexcept;
	virtual ~LandGenerator() = default;
	void setup( const map2D_f & shoreMap );
	void updateCellsIndirectBuffer( const Frustum & frustum );

private:
	friend class LandRenderer;
	friend class LandFacade;

	void splitChunks( int chunkSize );
	void splitCellChunks( int chunkSize );
	void fillBufferData();
	void fillCellBufferData();
	void bufferData( BufferCollection & bufferCollection, 
					 GLfloat * buffer, 
					 size_t size );
	void setupVBOAttributes() noexcept;
	void setupVBOInstancedAttributes() noexcept;
	void addIndirectBufferData( GLuint * buffer, 
								GLuint & dataOffset, 
								GLuint numInstances, 
								GLuint instanceOffset ) noexcept;

	BufferCollection cellBuffers;
	map2D_f chunkMap;
	std::vector<TerrainTile> cellTiles;
	std::default_random_engine randomizer;
	std::vector<LandChunk> chunks;
	std::vector<LandChunk> cellChunks;
	GLuint cellPrimitiveCount;
};
