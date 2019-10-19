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

/**
* @brief constructor that assembles vertex out of the byte array
* @param data byte array containing vertex data
* @param index current vertex index in byte array (not the byte offset!)
*/
ModelVertex::ModelVertex( char * data, 
						  int index )
{
	size_t byteOffset = index * sizeof( ModelVertex );

	Position.x = *( reinterpret_cast<float*>( data + byteOffset ) );
	byteOffset += sizeof( Position.x );
	Position.y = *( reinterpret_cast<float*>( data + byteOffset ) );
	byteOffset += sizeof( Position.y );
	Position.z = *( reinterpret_cast<float*>( data + byteOffset ) );
	byteOffset += sizeof( Position.z );

	Normal.x = *( reinterpret_cast<float*>( data + byteOffset ) );
	byteOffset += sizeof( Normal.x );
	Normal.y = *( reinterpret_cast<float*>( data + byteOffset ) );
	byteOffset += sizeof( Normal.y );
	Normal.z = *( reinterpret_cast<float*>( data + byteOffset ) );
	byteOffset += sizeof( Normal.z );

	TexCoords.x = *( reinterpret_cast<float*>( data + byteOffset ) );
	byteOffset += sizeof( TexCoords.x );
	TexCoords.y = *( reinterpret_cast<float*>( data + byteOffset ) );
	byteOffset += sizeof( TexCoords.y );

	Tangent.x = *( reinterpret_cast<float*>( data + byteOffset ) );
	byteOffset += sizeof( Tangent.x );
	Tangent.y = *( reinterpret_cast<float*>( data + byteOffset ) );
	byteOffset += sizeof( Tangent.y );
	Tangent.z = *( reinterpret_cast<float*>( data + byteOffset ) );
	byteOffset += sizeof( Tangent.z );

	Bitangent.x = *( reinterpret_cast<float*>( data + byteOffset ) );
	byteOffset += sizeof( Bitangent.x );
	Bitangent.y = *( reinterpret_cast<float*>( data + byteOffset ) );
	byteOffset += sizeof( Bitangent.y );
	Bitangent.z = *( reinterpret_cast<float*>( data + byteOffset ) );
	byteOffset += sizeof( Bitangent.z );

	TexIndices.x = *( reinterpret_cast<unsigned int*>( data + byteOffset ) );
	byteOffset += sizeof( TexIndices.x );
	TexIndices.y = *( reinterpret_cast<unsigned int*>( data + byteOffset ) );
	byteOffset += sizeof( TexIndices.y );
}
