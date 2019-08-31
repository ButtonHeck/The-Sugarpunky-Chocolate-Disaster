/*
 * Copyright 2019 Ilya Malgin
 * ShadowVolumeRenderer.h
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
 * Purpose: contains declaration for ShadowVolumeRenderer class
 * @version 0.1.0
 */

#pragma once

#include "ShadowVolume"
#include "BufferCollection"

/**
* @brief debug utility renderer for shadow volume regions boxes
* @todo remove this in release version of the game
*/
class ShadowVolumeRenderer
{
public:
	ShadowVolumeRenderer( ShadowVolume & shadowVolume );
	void bufferExpectedVolumes();
	void renderExpectedVolume( int index );
	void bufferActualVolumes();
	void renderActualVolume( int index );
	void renderLightSource( int index ) noexcept;

private:
	constexpr static unsigned int VOLUMES_INDICES_COUNT = 24;
	constexpr static float LIGHT_SOURCE_POSITION_POINT_SIZE = 12.0f;

	ShadowVolume & shadowVolume;
	BufferCollection expectedVolumesGLBuffers;
	/* 3 stands for number of floats (x,y,z) of each vertex
	 * and we need them twice because expected vertices duplicates and translates to ShadowVolume::BOX_MIN_HEIGHT in y-axis
	 * to form the box itself
	 */
	GLfloat expectedVolumesVertices[NUM_SHADOW_LAYERS * ShadowVolume::BOX_EXPECTED_VERTICES * 3 * 2] = { 0 };

	BufferCollection actualVolumesGLBuffers;
	/* +1 stands for one vertex of a sun position(relative to the box itself)
	 * and 3 is again - the number of floats of each vertex
	 */
	GLfloat actualVolumeVertices[NUM_SHADOW_LAYERS * ( ShadowVolume::BOX_ACTUAL_VERTICES + 1 ) * 3] = { 0 };

	GLuint volumesIndices[VOLUMES_INDICES_COUNT] = { 0,1,2,2,3,0,
													1,5,6,6,2,1,
													5,4,7,7,6,5,
													4,0,3,3,7,4 };
};
