/*
 * Copyright 2019 Ilya Malgin
 * Mesh.h
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
 * Purpose: contains declaration for Mesh class
 * @version 0.1.0
 */

#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class aiMesh;

/**
 * @brief Represents storage for a mesh vertices and indices.
 * Responsible for keeping necessary mesh data, object creation is done via static factory method
 */
class Mesh
{
public:
	/**
	 * @brief Structure of any mesh vertex in the game
	 */
	struct Vertex
	{
		glm::vec3  Position;
		glm::vec3  Normal;
		glm::vec2  TexCoords;
		glm::vec3  Tangent;
		glm::vec3  Bitangent;
		glm::uvec2 TexIndices;
	};

	static Mesh generate( const aiMesh * mesh, 
						  GLuint & diffuseSamplerIndex, 
						  GLuint & specularSamplerIndex, 
						  GLuint indexOffset );
	const std::vector<Vertex> & getVertices() const noexcept;
	const std::vector<unsigned int> & getIndices() const noexcept;

private:
	Mesh() = default;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};
