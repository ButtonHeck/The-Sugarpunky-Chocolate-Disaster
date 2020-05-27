/*
 * Copyright 2019 Ilya Malgin
 * LensFlareRenderer.cpp
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
 * Purpose: contains definitions for LensFlareRenderer class
 * @version 0.1.0
 */

#include "LensFlareRenderer"
#include "BufferCollection"
#include "RendererState"

/**
 * @param basicGLBuffer buffer collection to fetch VAO from during rendering
 */
LensFlareRenderer::LensFlareRenderer( BufferCollection & basicGLBuffer ) noexcept
	: basicGLBuffer( basicGLBuffer )
{}

/**
 * @brief setup appropriate OpenGL state and draw given number of points
 * @param numFlares flare points count to be drawn
 */
void LensFlareRenderer::draw( unsigned int numFlares )
{
	basicGLBuffer.bind( VAO );

	RendererState::enableState( GL_PROGRAM_POINT_SIZE );
	glDepthMask( GL_FALSE ); //do not write to depth buffer otherwise flares would occlude each other
	glBlendFunc( GL_SRC_ALPHA, GL_ONE ); //apply additive blending
	glDrawArrays( GL_POINTS, 0, numFlares );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); //switch back to default blending function
	glDepthMask( GL_TRUE ); //switch back depth writing
	RendererState::disableState( GL_PROGRAM_POINT_SIZE );
}
