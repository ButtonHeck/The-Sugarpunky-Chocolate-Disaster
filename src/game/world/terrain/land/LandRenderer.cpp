/*
 * Copyright 2019 Ilya Malgin
 * LandRenderer.cpp
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
 * Purpose: contains definitions for LandRenderer class
 * @version 0.1.0
 */

#include "LandRenderer"
#include "LandGenerator"
#include "BenchmarkTimer"

/**
* @brief plain ctor
* @param generator land generator to fetch GL objects from
*/
LandRenderer::LandRenderer( LandGenerator & generator ) noexcept
	: generator( generator )
{}

/**
* @brief sends draw calls to OpenGL, instanced for square segments and instanced indirect for cells
*/
void LandRenderer::render()
{
	//square chunks are better to render without frustum culling
	{
		BENCHMARK( "LandRenderer: draw chunks", true );
		generator.basicGLBuffers.bind( VAO );
		glDrawElementsInstanced( GL_TRIANGLES, VERTICES_PER_QUAD, GL_UNSIGNED_BYTE, 0, generator.tiles.size() );
	}

	//these ones should probably be culled with multiDrawIndirect
	{
		BENCHMARK( "LandRenderer: draw cells", true );
		generator.cellBuffers.bind( VAO | DIBO );
		glMultiDrawElementsIndirect( GL_TRIANGLES, GL_UNSIGNED_BYTE, 0, generator.cellPrimitiveCount, 0 );
	}
}
