/*
 * Copyright 2019 Ilya Malgin
 * LandChunk.h
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
 * Purpose: contains declaration and inline functions for LandChunk class
 * @version 0.1.0
 */

#pragma once

#include "Chunk"

/**
* @brief Land chunk subclass. Keeps data about single tile instances count and their offset.
*/
class LandChunk : public Chunk
{
public:
	LandChunk( unsigned int left, 
			   unsigned int right, 
			   unsigned int top, 
			   unsigned int bottom, 
			   unsigned int offset, 
			   unsigned int instances ) noexcept;

	//instance offset
	void setInstanceOffset( unsigned int offset ) noexcept;
	unsigned int getInstanceOffset() const noexcept;

	//num instances
	void setNumInstances( unsigned int instances ) noexcept;
	unsigned int getNumInstances() const noexcept;

private:
	unsigned int instanceOffset;
	unsigned int numInstances;
};

inline void LandChunk::setInstanceOffset( unsigned int offset ) noexcept
{
	this->instanceOffset = offset;
}

inline unsigned int LandChunk::getInstanceOffset() const noexcept
{
	return instanceOffset;
}

inline void LandChunk::setNumInstances( unsigned int instances ) noexcept
{
	this->numInstances = instances;
}

inline unsigned int LandChunk::getNumInstances() const noexcept
{
	return numInstances;
}
