/*
 * Copyright 2019 Ilya Malgin
 * ShoreFacade.cpp
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
 * Purpose: contains definitions for ShoreFacade class
 * @version 0.1.0
 */

#include "ShoreFacade"
#include "BenchmarkTimer"

/**
* @brief plain ctor. Creates all member submodules
* @param renderShader shader program used for onscreen rendering
* @param normalsShader shader program used for onscreen normals rendering
* @param waterMap map of the water
*/
ShoreFacade::ShoreFacade( Shader & renderShader, 
						  Shader & normalsShader, 
						  const map2D_f & waterMap )
	: shader( renderShader, normalsShader )
	, generator( waterMap )
	, renderer( generator )
{}

/**
* @brief delegates map generation routine to generator
*/
void ShoreFacade::setup()
{
	generator.setup();
}

/**
* @brief delegates serialization call to generator
* @param output file stream to write data to
*/
void ShoreFacade::serialize( std::ofstream & output )
{
	generator.serialize( output );
}

/**
* @brief delegates deserialization call to generator
* @param input file stream to read data from
*/
void ShoreFacade::deserialize( std::ifstream & input )
{
	generator.deserialize( input );
}

/**
* @brief handles shores drawing routine: prepares shader and delegates draw command to renderer
* @param lightDir sunlight direction vector
* @param lightSpaceMatrices array of "projection * view" matrices from the Sun point of view
* @param projectionView "projection * view" matrix
* @param useShadows indicator of whether shadows should be calculated
* @param useDebugRender indicator of whether to render additional visualizations for debugging
* @param useClipDistanceReflection indicator of whether clip distance will be used by OpenGL for reflection rendering
* @param useClipDistanceRefraction indicator of whether clip distance will be used by OpenGL for refraction rendering
*/
void ShoreFacade::draw( const glm::vec3 & lightDir,
						const std::array<glm::mat4, NUM_SHADOW_LAYERS> & lightSpaceMatrices,
						const glm::mat4 & projectionView,
						bool useShadows,
						bool useDebugRender,
						bool useClipDistanceReflection,
						bool useClipDistanceRefraction )
{
	shader.update( lightDir,
				   lightSpaceMatrices,
				   projectionView,
				   useShadows,
				   useClipDistanceReflection,
				   useClipDistanceRefraction );
	{
		BENCHMARK( "ShoreRenderer: draw", true );
		shader.debugRenderMode( false );
		renderer.render();
	}

	if( useDebugRender )
	{
		shader.debugRenderMode( true );
		renderer.debugRender( GL_TRIANGLES );
		shader.updateNormals( projectionView );
		renderer.debugRender( GL_POINTS );
	}
}

/**
* @brief delegates draw call to renderer, supposing that no dedicated shader for shore should be updated
* @note while rendering shore to the depthmap it's necessary to disable face culling, otherwise most polygons would be ignored
*/
void ShoreFacade::drawDepthmap()
{
	glDisable( GL_CULL_FACE );
	renderer.render();
	glEnable( GL_CULL_FACE );
}

const map2D_f & ShoreFacade::getMap() const noexcept
{
	return generator.getMap();
}
