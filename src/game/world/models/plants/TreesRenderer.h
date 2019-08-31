/*
 * Copyright 2019 Ilya Malgin
 * TreesRenderer.h
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
 * Purpose: contains declaration for TreesRenderer class
 * @version 0.1.0
 */

#pragma once

#include <vector>

class Model;

/**
 * @brief Renderer wrapper for the entire bunch of land trees and hill trees models.
 * Responsible for delegating draw calls to given set of models
 */
class TreesRenderer
{
public:
	TreesRenderer() = default;
	void render( std::vector<Model> & landModels, 
				 std::vector<Model> & hillModels, 
				 bool isShadow );
	void renderWorldReflection( std::vector<Model> & landModels, 
								std::vector<Model> & hillModels );
};
