/*
 * Copyright 2019 Ilya Malgin
 * LandFacade.cpp
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
 * Purpose: contains definitions for LandFacade class
 * @version 0.1.0
 */

#include "LandFacade"

/**
* @brief plain ctor. Creates all the member submodules
* @param renderShader shader prograsm used during rendering
*/
LandFacade::LandFacade( Shader & renderShader ) noexcept
	: shader( renderShader )
	, generator()
	, renderer( generator )
{}

/**
* @brief delegates map generation routine to generator
* @param shoreMap map of the shore tiles
*/
void LandFacade::setup( const map2D_f & shoreMap )
{
	generator.setup( shoreMap );
}

/**
* @brief delegates serialization call to generator
* @param output file stream to write data to
*/
void LandFacade::serialize( std::ofstream & output )
{
	generator.serialize( output );
}

/**
* @brief delegates deserialization call to generator
* @param input file stream to read data from
*/
void LandFacade::deserialize( std::ifstream & input )
{
	generator.deserialize( input );
}

/**
* @brief handles land drawing routine: prepares shader and delegates draw command to renderer
* @param lightDir sunlight direction vector
* @param lightSpaceMatrices array of "projection * view" matrices from the Sun point of view
* @param projectionView "projection * view" matrix
* @param useShadows indicator of whether shadows should be calculated
*/
void LandFacade::draw( const glm::vec3 & lightDir,
					   const std::array<glm::mat4, NUM_SHADOW_LAYERS> & lightSpaceMatrices,
					   const glm::mat4 & projectionView,
					   bool useShadows )
{
	shader.update( lightDir, lightSpaceMatrices, projectionView, useShadows );
	renderer.render();
}

const map2D_f & LandFacade::getMap() const noexcept
{
	return generator.getMap();
}

/**
* @brief delegates update indirect buffer command to generator
* @param frustum view frustum of the camera
*/
void LandFacade::updateCellsIndirectBuffer( const Frustum & frustum )
{
	generator.updateCellsIndirectBuffer( frustum );
}
