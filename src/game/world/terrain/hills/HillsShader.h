/*
 * Copyright 2019 Ilya Malgin
 * HillsShader.h
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
 * Purpose: contains declaration for HillsShader class
 * @version 0.1.0
 */

#pragma once

#include "GraphicsSettings"

#include <array>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Shader;
class Frustum;

/**
* @brief shader manager for hills. Responsible for managing different hills related shader programs' settings
* and updating them.
*/
class HillsShader
{
public:
	HillsShader( Shader & renderShader, 
				 Shader & cullingShader, 
				 Shader & normalsShader ) noexcept;
	void setupCulling();
	void update( const glm::vec3 & lightDir,
				 const std::array<glm::mat4, NUM_SHADOW_LAYERS> & lightSpaceMatrices,
				 const glm::mat4 & projectionView,
				 const glm::vec3 & viewPosition,
				 const Frustum & viewFrustum,
				 float maxHillHeight,
				 bool useFrustumCulling,
				 bool useShadows );
	void updateNormals( const glm::mat4 & projectionView );
	void debugRenderMode( bool enable );

private:
	friend class HillsRenderer;

	Shader & renderShader;
	Shader & cullingShader;
	/** @todo for visual debugging only, delete this in release version of the game */
	Shader & normalsShader;
};
