/*
 * Copyright 2019 Ilya Malgin
 * LandChunk.cpp
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
 * Purpose: contains definitions for LandChunk class
 * @version 0.1.0
 */

#include "LandChunk"

/**
* @brief plain ctor
* @param offset instance offset, for indirect buffer usage
* @param instances number of tile instances in chunk, for indirect buffer usage
*/
LandChunk::LandChunk( unsigned int left, 
					  unsigned int right, 
					  unsigned int top, 
					  unsigned int bottom, 
					  unsigned int offset, 
					  unsigned int instances ) noexcept
	: Chunk( left, right, top, bottom )
	, instanceOffset( offset )
	, numInstances( instances )
{}
