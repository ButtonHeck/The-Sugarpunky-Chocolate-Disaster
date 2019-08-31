/*
 * Copyright 2019 Ilya Malgin
 * FrustumRenderer.h
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
 * Purpose: contains declaration for FrustumRenderer class
 * @version 0.1.0
 */

#pragma once

#include "Frustum"
#include "BufferCollection"

/**
* @brief utility class for rendering view frustum's bounds. Intended to be used for visual debugging only
* @todo remove this class in the release version of the game
*/
class FrustumRenderer
{
public:
	FrustumRenderer( const Frustum & frustum );
	void render();

private:
	const GLuint INDICES[Frustum::NUMBER_OF_PLANES * VERTICES_PER_QUAD] =
	{ 0,1,2,2,3,0,
	 1,5,6,6,2,1,
	 5,4,7,7,6,5,
	 4,0,3,3,7,4,
	 3,2,6,6,7,3,
	 1,5,4,4,0,1 };

	const Frustum & frustum;
	BufferCollection basicGLBuffers;
	float vertices[3 * Frustum::NUMBER_OF_DISTINCT_VERTICES] = { 0.0f };
};
