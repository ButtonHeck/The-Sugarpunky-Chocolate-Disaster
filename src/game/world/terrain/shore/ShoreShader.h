/*
 * Copyright 2019 Ilya Malgin
 * ShoreShader.h
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
 * Purpose: contains declaration for ShoreShader class
 * @version 0.1.0
 */

#pragma once

#include "GraphicsSettings"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <array>

class Shader;

/**
* @brief shader manager for shore. Responsible for updating shore shader uniforms
*/
class ShoreShader
{
public:
	ShoreShader( Shader & renderShader, 
				 Shader & normalsShader ) noexcept;
	void update( const glm::vec3 & lightDir,
				 const std::array<glm::mat4, NUM_SHADOW_LAYERS> & lightSpaceMatrices,
				 const glm::mat4 & projectionView,
				 bool useShadows,
				 bool useClipDistanceReflection,
				 bool useClipDistanceRefraction );
	void updateNormals( const glm::mat4 & projectionView );
	void debugRenderMode( bool enable );

private:
	Shader & renderShader;
	/** @todo remove this in release version of the game */
	Shader & normalsShader;
};
