/*
 * Copyright 2019 Ilya Malgin
 * ModelRenderer.cpp
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
 * Purpose: contains definitions for ModelRenderer class
 * @version 0.1.0
 */

#include "ModelRenderer"
#include "BufferCollection"
#include "BenchmarkTimer"

/**
* @brief plain ctor
* @param basicGLBuffers onscreen rendering buffer collection
* @param depthmapDIBO offscreen indirect draw buffer for depthmap rendering
* @param reflectionDIBO onscreen indirect draw buffer for world reflection rendering
*/
ModelRenderer::ModelRenderer( BufferCollection & basicGLBuffers,
							  BufferCollection & depthmapDIBO,
							  BufferCollection & reflectionDIBO ) noexcept
	: basicGLBuffers( basicGLBuffers )
	, depthmapDIBO( depthmapDIBO )
	, reflectionDIBO( reflectionDIBO )
{}

/**
* @brief sends draw calls to OpenGL depending on the current mode
* @param isDepthmap rendering mode
* @param primCount number of instances to render
*/
void ModelRenderer::render( MODEL_INDIRECT_BUFFER_TYPE type, 
							GLsizei primCount )
{
	basicGLBuffers.bind( VAO );
	if( type == PLAIN_ONSCREEN )
	{
		basicGLBuffers.bind( DIBO );
		BENCHMARK( "Model: draw", true );
		glMultiDrawElementsIndirect( GL_TRIANGLES, GL_UNSIGNED_INT, 0, primCount, 0 );
	}
	else if( type == DEPTHMAP_OFFSCREEN )
	{
		depthmapDIBO.bind( DIBO );
		BENCHMARK( "Model: draw shadows", true );
		glMultiDrawElementsIndirect( GL_TRIANGLES, GL_UNSIGNED_INT, 0, primCount, 0 );
	}
	else
	{
		reflectionDIBO.bind( DIBO );
		BENCHMARK( "Model: draw reflections", true );
		glMultiDrawElementsIndirect( GL_TRIANGLES, GL_UNSIGNED_INT, 0, primCount, 0 );
	}
}

/**
* @brief sends draw call to OpenGL to render model as the singleton
* @param numIndices number of indices in the model to render
*/
void ModelRenderer::renderOneInstance( GLsizei numIndices )
{
	basicGLBuffers.bind( VAO );
	glDrawElements( GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0 );
}
