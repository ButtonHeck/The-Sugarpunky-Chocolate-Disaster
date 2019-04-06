/*
 * Copyright 2019 Ilya Malgin
 * ModelChunk.cpp
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
 * Purpose: contains definitions for ModelChunk class
 * @version 0.1.0
 */

#include "game/world/models/ModelChunk.h"

ModelChunk::ModelChunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom, float height)
  :
    Chunk(left, right, top, bottom, height)
{}

/**
 * @brief Check whether this chunk is within given view frustum at the moment
 * @param frustum view frustum to check
 * @return true if a chunk is inside a frustum, false otherwise
 */
bool ModelChunk::isInsideFrustum(const Frustum &frustum) const
{
  BENCHMARK("ModelChunk: isInsideFrustum", true);
  return frustum.isInside(midPointX - HALF_CHUNK_SIZE, height, midPointZ + HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET) ||
         frustum.isInside(midPointX + HALF_CHUNK_SIZE, height, midPointZ + HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET) ||
         frustum.isInside(midPointX + HALF_CHUNK_SIZE, height, midPointZ - HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET) ||
         frustum.isInside(midPointX - HALF_CHUNK_SIZE, height, midPointZ - HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET);
}
