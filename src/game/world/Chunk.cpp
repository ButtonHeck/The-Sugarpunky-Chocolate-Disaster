/*
 * Copyright 2019 Ilya Malgin
 * Chunk.cpp
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
 * Purpose: contains definition for Chunk class
 * @version 0.1.0
 */

#include "Chunk"
#include "SceneSettings"
#include "Frustum"

/**
* @brief plain ctor
* @note height is defaulted by 0.0f value
*/
Chunk::Chunk( unsigned int left,
			  unsigned int right,
			  unsigned int top,
			  unsigned int bottom,
			  float height ) noexcept
	: left( left )
	, right( right )
	, top( top )
	, bottom( bottom )
	, height( height )
{
	midPointX = -HALF_WORLD_WIDTH + ( right - left ) / 2.0f + left;
	midPointZ = -HALF_WORLD_HEIGHT + ( bottom - top ) / 2.0f + top;
}

/**
 * @brief Check whether this chunk is within given view frustum at the moment
 * @param frustum view frustum to check
 * @param cullingOffset offset applied to frustum sides
 * @return true if a chunk is inside a frustum, false otherwise
 */
bool Chunk::isInsideFrustum( const Frustum & frustum,
							 float cullingOffset ) const
{
	return frustum.isInside( midPointX - HALF_CHUNK_SIZE, height, midPointZ + HALF_CHUNK_SIZE, cullingOffset ) ||
		   frustum.isInside( midPointX + HALF_CHUNK_SIZE, height, midPointZ + HALF_CHUNK_SIZE, cullingOffset ) ||
		   frustum.isInside( midPointX + HALF_CHUNK_SIZE, height, midPointZ - HALF_CHUNK_SIZE, cullingOffset ) ||
		   frustum.isInside( midPointX - HALF_CHUNK_SIZE, height, midPointZ - HALF_CHUNK_SIZE, cullingOffset ) ||
		   frustum.isInside( midPointX - HALF_CHUNK_SIZE, 0.0f, midPointZ + HALF_CHUNK_SIZE, cullingOffset ) ||
		   frustum.isInside( midPointX + HALF_CHUNK_SIZE, 0.0f, midPointZ + HALF_CHUNK_SIZE, cullingOffset ) ||
		   frustum.isInside( midPointX + HALF_CHUNK_SIZE, 0.0f, midPointZ - HALF_CHUNK_SIZE, cullingOffset ) ||
		   frustum.isInside( midPointX - HALF_CHUNK_SIZE, 0.0f, midPointZ - HALF_CHUNK_SIZE, cullingOffset );
}
