/*
 * Copyright 2019 Ilya Malgin
 * ModelVertex.h
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
 * Purpose: contains declaration for ModelVertex struct
 * @version 0.1.0
 */

#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

/**
* @brief Model vertex internal structure
*/
struct ModelVertexImpl
{
	glm::vec3  Position;
	glm::vec3  Normal;
	glm::vec2  TexCoords;
	glm::vec3  Tangent;
	glm::vec3  Bitangent;
	glm::uvec2 TexIndices;
};
constexpr size_t MODEL_VERTEX_SIZE = sizeof( ModelVertexImpl );

/**
* @brief Representation of a model vertex
*/
struct ModelVertex
{
	ModelVertex( char * data,
				 int index );
	union
	{
		ModelVertexImpl impl;
		char vertexRaw[MODEL_VERTEX_SIZE];
	};	
};