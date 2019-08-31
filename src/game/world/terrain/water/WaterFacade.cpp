/*
 * Copyright 2019 Ilya Malgin
 * WaterFacade.cpp
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
 * Purpose: contains definitions for WaterFacade class
 * @version 0.1.0
 */

#include "WaterFacade"

/**
* @brief plain ctor
* @param renderShader shader program used for onscreen rendering
* @param cullingShader shader program used for offscreen rendering with frustum culling
* @param normalsShader shader program used for onscreen rendering of water normals
*/
WaterFacade::WaterFacade( Shader & renderShader, 
						  Shader & cullingShader, 
						  Shader & normalsShader )
	: shaders( renderShader, cullingShader, normalsShader )
	, generator( shaders )
	, renderer( shaders, generator )
{}

/**
* @brief delegates map generation routine to generator
*/
void WaterFacade::setup()
{
	generator.setup();
}

/**
* @brief delegates map post-process routine to generator
*/
void WaterFacade::setupConsiderTerrain()
{
	generator.setupConsiderTerrain();
}

/**
* @brief delegates serialization call to generator
* @param output file stream to write data to
*/
void WaterFacade::serialize( std::ofstream & output )
{
	generator.serialize( output );
}

/**
* @brief delegates deserialization call to generator
* @param input file stream to read data from
*/
void WaterFacade::deserialize( std::ifstream & input )
{
	generator.deserialize( input );
}

/**
* @brief handles water drawing routine: prepares shader and delegates draw command to renderer
* @param lightDir sunlight direction vector
* @param lightSpaceMatrices array of "projection * view" matrices from the Sun point of view
* @param projectionView "projection * view" matrix
* @param viewPosition player's camera current position
* @param viewFrustum player's camera view frustum
* @param useFrustumCulling defines whether frustum culling mode is on
* @param useDebugRender defines whether debug rendering mode is on
*/
void WaterFacade::draw( const glm::vec3 & lightDir,
						const std::array<glm::mat4, NUM_SHADOW_LAYERS> & lightSpaceMatrices,
						const glm::mat4 & projectionView,
						const glm::vec3 & viewPosition,
						const Frustum & viewFrustum,
						bool useFrustumCulling,
						bool useDebugRender )
{
	shaders.update( lightDir,
					lightSpaceMatrices,
					projectionView,
					viewPosition,
					viewFrustum,
					useFrustumCulling );
	shaders.debugRenderMode( false );
	renderer.render( useFrustumCulling );

	//temporary code for visual debugging
	if( useDebugRender )
	{
		shaders.debugRenderMode( true );
		renderer.debugRender( GL_TRIANGLES );
		shaders.updateNormals( projectionView );
		renderer.debugRender( GL_POINTS );
	}
}

const map2D_f & WaterFacade::getMap() const noexcept
{
	return generator.getMap();
}

/**
* @brief delegates query call to generator
*/
bool WaterFacade::hasWaterInFrame() const noexcept
{
	return renderer.anySamplesPassed();
}
