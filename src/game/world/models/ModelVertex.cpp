/*
 * Copyright 2019 Ilya Malgin
 * ModelVertex.cpp
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
 * Purpose: contains definition for ModelVertex struct
 * @version 0.1.0
 */

#include "ModelVertex"

#include <cstring>

/**
* @brief constructor that assembles vertex out of the byte array
* @param data byte array containing vertex data
* @param index current vertex index in byte array (not the byte offset!)
* @note using 'union' lets us just do memcpy
*/
ModelVertex::ModelVertex( char * data, 
						  int index )
{
	size_t byteOffset = index * MODEL_VERTEX_SIZE;
	memcpy( vertexRaw, data + byteOffset, MODEL_VERTEX_SIZE );
}
