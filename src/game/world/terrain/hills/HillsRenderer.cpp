/*
 * Copyright 2019 Ilya Malgin
 * HillsRenderer.cpp
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
 * Purpose: contains definitions for HillsRenderer class
 * @version 0.1.0
 */

#include "HillsRenderer"
#include "HillsGenerator"
#include "HillsShader"
#include "Shader"
#include "BenchmarkTimer"
#include "RendererState"

/**
* @brief plain ctor.
* @param shaders shader manager
* @param generator hills generator, used to fetch GL buffer objects for rendering
*/
HillsRenderer::HillsRenderer( HillsShader & shaders, 
							  HillsGenerator & generator ) noexcept
	: shaders( shaders )
	, generator( generator )
{}

/**
* @brief manages draw calls to OpenGL and optionally switches to custom frustum culling mode rendering
* @param useFrustumCulling indicator of frustum culling mode
* @param viewAcceleration a pair of camera's acceleration values (on X and Y axis), used for frustum culling
*/
void HillsRenderer::render( bool useFrustumCulling, 
							const glm::vec2 & viewAcceleration )
{
	if( useFrustumCulling )
	{
	    /*
	    * by default transform feedback rendering takes place every FEEDBACK_UPDATE_FRAMERATE frames,
	    * but the more acceleration values of the camera are, the more frequent feedback rendering should occur.
	    * Thus, adjust framerate of feedback rendering depend on camera's current acceleration (max X or Y)
	    */
		const unsigned int FEEDBACK_UPDATE_FRAMERATE = 12;
		float accelerationAbs = glm::max( glm::abs( viewAcceleration.x ), glm::abs( viewAcceleration.y ) );
		int updateIncrement = 1;
		if( accelerationAbs > 0.5f && accelerationAbs <= 5.0f )
		{
			updateIncrement = 4;
		}
		else if( accelerationAbs > 5.0f && accelerationAbs <= 10.0f )
		{
			updateIncrement = 5;
		}
		else if( accelerationAbs > 10.0f )
		{
			updateIncrement = 6;
		}

		//made static in order to keep this counter value from call to call
		static unsigned int updateTransformFeedbackCounter = 0;
		GLuint transformFeedback = generator.culledBuffers.get( TFBO );
		if( updateTransformFeedbackCounter == 0 )
		{
			//render hills offscreen with transform feedback
			BENCHMARK( "HillsRenderer: draw to TFB", true );
			shaders.cullingShader.use();
			generator.basicGLBuffers.bind( VAO );
			RendererState::enableState( GL_RASTERIZER_DISCARD );
			glBindTransformFeedback( GL_TRANSFORM_FEEDBACK, transformFeedback );
			glBeginTransformFeedback( GL_TRIANGLES );
			glDrawElements( GL_TRIANGLES, generator.tiles.size() * VERTICES_PER_QUAD, GL_UNSIGNED_INT, 0 );
			glEndTransformFeedback();
			RendererState::disableState( GL_RASTERIZER_DISCARD );
		}
		updateTransformFeedbackCounter += updateIncrement;
		if( updateTransformFeedbackCounter >= FEEDBACK_UPDATE_FRAMERATE )
		{
			updateTransformFeedbackCounter = 0;
		}
		{
			//render feedback onscreen
			BENCHMARK( "HillsRenderer: draw from TFB", true );
			shaders.renderShader.use();
			generator.culledBuffers.bind( VAO );
			glDrawTransformFeedback( GL_TRIANGLES, transformFeedback );
		}
	}
	else
	{
		//plain onscreen rendering
		BENCHMARK( "HillsRenderer: draw", true );
		shaders.renderShader.use();
		generator.basicGLBuffers.bind( VAO );
		glDrawElements( GL_TRIANGLES, generator.tiles.size() * VERTICES_PER_QUAD, GL_UNSIGNED_INT, 0 );
	}
}

/**
* @brief render hills to depthmap
*/
void HillsRenderer::renderDepthmap()
{
	BENCHMARK( "HillsRenderer: render depthmap", true );
	generator.basicGLBuffers.bind( VAO );
	glDrawElements( GL_TRIANGLES, generator.tiles.size() * VERTICES_PER_QUAD, GL_UNSIGNED_INT, 0 );
}

/**
* @brief utulity function that renders additional visual information such as normals/tangents/bitangents, hills grid etc.
* @param primitiveType GL enumerated value of a geometry primitive to use for rendering
* @todo remove this function in release version of the game
*/
void HillsRenderer::debugRender( GLenum primitiveType )
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
