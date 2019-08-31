/*
 * Copyright 2019 Ilya Malgin
 * UnderwaterFacade.cpp
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
 * Purpose: contains definitions for UnderwaterFacade class
 * @version 0.1.0
 */

#include "UnderwaterFacade"
#include "BenchmarkTimer"

/**
* @brief plain ctor
* @param renderShader shader program used for rendering
*/
UnderwaterFacade::UnderwaterFacade( Shader & renderShader ) noexcept
	: shader( renderShader )
	, surface()
	, renderer( surface )
{}

/**
* @brief handles rendering routine
* @param lightDir sunlight direction vector
* @param projectionView "projection * view" matrix
* @param useDesaturation define whether to apply desaturation
*/
void UnderwaterFacade::draw( const glm::vec3 & lightDir,
							 const glm::mat4 & projectionView,
							 bool useDesaturation )
{
	BENCHMARK( "UnderwaterRenderer: draw", true );
	shader.update( lightDir, projectionView, useDesaturation );
	renderer.render();
}
