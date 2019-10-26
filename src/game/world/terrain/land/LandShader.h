/*
 * Copyright 2019 Ilya Malgin
 * LandShader.h
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
 * Purpose: contains declaration for LandShader class
 * @version 0.1.0
 */

#pragma once

#include "GraphicsConstants"

#include <array>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Shader;

/**
* @brief shader manager for lands, Responsible for updating land shader uniforms
*/
class LandShader
{
public:
	LandShader( Shader & renderShader ) noexcept;
	void update( const glm::vec3 & lightDir,
				 const std::array<glm::mat4, NUM_SHADOW_LAYERS> & lightSpaceMatrices,
				 const glm::mat4 & projectionView,
				 bool useShadows );

private:
	Shader & renderShader;
};
