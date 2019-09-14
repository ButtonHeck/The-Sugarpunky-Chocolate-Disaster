/*
 * Copyright 2019 Ilya Malgin
 * Chunk.h
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
 * Purpose: contains declaration and inline functions for Chunk class
 * @version 0.1.0
 */

#pragma once

#include <glm/vec2.hpp>

class Frustum;

/**
* @brief base class for different types of chunks.
* Contains information only about its coordinates, middle point and approximate height on map(a chunk itself is a 2D square)
*/
class Chunk
{
public:
	Chunk( unsigned int left, 
		   unsigned int right, 
		   unsigned int top, 
		   unsigned int bottom, 
		   float height = 0.0f ) noexcept;
	glm::vec2 getMidPoint() const;
	unsigned int getLeft() const noexcept;
	unsigned int getRight() const noexcept;
	unsigned int getTop() const noexcept;
	unsigned int getBottom() const noexcept;
	float getHeight() const noexcept;
	void setHeight( float height ) noexcept;
	bool isInsideFrustum( const Frustum & frustum, 
						  float cullingOffset ) const;

protected:
	unsigned int left;
	unsigned int right;
	unsigned int top;
	unsigned int bottom;
	float midPointX;
	float midPointZ;
	float height;
};

inline glm::vec2 Chunk::getMidPoint() const
{
	return glm::vec2( midPointX, midPointZ );
}

inline unsigned int Chunk::getLeft() const noexcept
{
	return left;
}

inline unsigned int Chunk::getRight() const noexcept
{
	return right;
}

inline unsigned int Chunk::getTop() const noexcept
{
	return top;
}

inline unsigned int Chunk::getBottom() const noexcept
{
	return bottom;
}

inline float Chunk::getHeight() const noexcept
{
	return height;
}

inline void Chunk::setHeight( float height ) noexcept
{
	this->height = height;
}
