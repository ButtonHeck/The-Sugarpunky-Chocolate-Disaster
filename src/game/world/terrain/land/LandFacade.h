/*
 * Copyright 2019 Ilya Malgin
 * LandFacade.h
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
 * Purpose: contains declaration for LandFacade class
 * @version 0.1.0
 */

#pragma once

#include "LandGenerator"
#include "LandShader"
#include "LandRenderer"

/**
* @brief facade for land related code. Responsible for delegating tasks to member objects
*/
class LandFacade
{
public:
	LandFacade( Shader & renderShader ) noexcept;
	void setup( const map2D_f & shoreMap );
	void serialize( std::ofstream & output );
	void deserialize( std::ifstream & input );
	void draw( const glm::vec3 & lightDir,
			   const std::array<glm::mat4, NUM_SHADOW_LAYERS> & lightSpaceMatrices,
			   const glm::mat4 & projectionView,
			   bool useShadows );
	const map2D_f & getMap() const noexcept;
	void updateCellsIndirectBuffer( const Frustum & frustum );

private:
	LandShader shader;
	LandGenerator generator;
	LandRenderer renderer;
};
