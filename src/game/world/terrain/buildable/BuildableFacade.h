/*
 * Copyright 2019 Ilya Malgin
 * BuildableFacade.h
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
 * Purpose: contains declaration for BuildableFacade class
 * @version 0.1.0
 */

#pragma once

#include "BuildableGenerator"
#include "BuildableShader"
#include "BuildableRenderer"

class MouseInputManager;

/**
* @brief facade for buildable and selected tiles on a map.
* Responsible for delegating buildable tiles related tasks among member objects
*/
class BuildableFacade
{
public:
	BuildableFacade( Shader & buildableRenderShader, 
					 Shader & selectedRenderShader ) noexcept;
	void setup( const map2D_f & landMap, 
				const map2D_f & hillsMap );
	void drawBuildable( const glm::mat4 & projectionView );
	void drawSelected( const glm::mat4 & projectionView, 
					   MouseInputManager & mouseInput );
	const map2D_f & getMap() const noexcept;

private:
	BuildableShader shader;
	BuildableGenerator generator;
	BuildableRenderer renderer;
};
