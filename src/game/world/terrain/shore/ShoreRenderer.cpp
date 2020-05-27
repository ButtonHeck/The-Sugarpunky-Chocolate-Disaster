/*
 * Copyright 2019 Ilya Malgin
 * ShoreRenderer.cpp
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
 * Purpose: contains definitions for ShoreRenderer class
 * @version 0.1.0
 */

#include "ShoreRenderer"
#include "ShoreGenerator"
#include "RendererState"

/**
* @brief plain ctor
* @param generator generator object to fetch vao from
*/
ShoreRenderer::ShoreRenderer( ShoreGenerator & generator ) noexcept
	: generator( generator )
{}

/**
* @brief sends draw call to OpenGL
*/
void ShoreRenderer::render()
{
	generator.basicGLBuffers.bind( VAO );
	glDrawElements( GL_TRIANGLES, generator.tiles.size() * VERTICES_PER_QUAD, GL_UNSIGNED_INT, 0 );
}

/**
* @brief sends draw call to OpenGL to draw additional visualizations for debugging
* @param primitiveType GL defined primitive type to render
* @todo remove this in release version of the game
*/
void ShoreRenderer::debugRender( GLenum primitiveType )
{
	generator.basicGLBuffers.bind( VAO );
	glLineWidth( 2.0f );
	RendererState::disableState( GL_CULL_FACE );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glDrawElements( primitiveType, generator.tiles.size() * VERTICES_PER_QUAD, GL_UNSIGNED_INT, 0 );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	RendererState::enableState( GL_CULL_FACE );
	glLineWidth( 1.0f );
}
