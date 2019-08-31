/*
 * Copyright 2019 Ilya Malgin
 * UnderwaterSurface.cpp
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
 * Purpose: contains definitions for UnderwaterSurface class
 * @version 0.1.0
 */

#include "UnderwaterSurface"
#include "SceneSettings"

/**
* @brief plain ctor, creates one huge quad representing the underwater surface
*/
UnderwaterSurface::UnderwaterSurface() noexcept
	: basicGLBuffers( VAO | VBO | EBO )
{
	constexpr GLfloat VERTICES[20] = {
	  -HALF_WORLD_WIDTH_F, UNDERWATER_TILE_Y, HALF_WORLD_HEIGHT_F, 0.0f,        0.0f,
	   HALF_WORLD_WIDTH_F, UNDERWATER_TILE_Y, HALF_WORLD_HEIGHT_F, WORLD_WIDTH, 0.0f,
	   HALF_WORLD_WIDTH_F, UNDERWATER_TILE_Y,-HALF_WORLD_HEIGHT_F, WORLD_WIDTH, WORLD_HEIGHT,
	  -HALF_WORLD_WIDTH_F, UNDERWATER_TILE_Y,-HALF_WORLD_HEIGHT_F, 0.0f,        WORLD_HEIGHT
	};
	basicGLBuffers.bind( VAO | VBO | EBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( QUAD_INDICES ), QUAD_INDICES, GL_STATIC_DRAW );
	glBufferData( GL_ARRAY_BUFFER, sizeof( VERTICES ), VERTICES, GL_STATIC_DRAW );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), 0 );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), (void*)( 3 * sizeof( GLfloat ) ) );
	BufferCollection::bindZero( VAO | VBO | EBO );
}
