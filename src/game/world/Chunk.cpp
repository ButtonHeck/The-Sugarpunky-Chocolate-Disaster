#include "game/world/Chunk.h"

Chunk::Chunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom, float height)
  :
    left(left),
    right(right),
    top(top),
    bottom(bottom),
    height(height)
{
  midPointX = -HALF_WORLD_WIDTH + (right - left) / 2.0f + left;
  midPointZ = -HALF_WORLD_HEIGHT + (bottom - top) / 2.0f + top;
}

/**
 * @brief Check whether this chunk is within given view frustum at the moment
 * @param frustum view frustum to check
 * @return true if a chunk is inside a frustum, false otherwise
 */
bool Chunk::isInsideFrustum(const Frustum &frustum) const
{
  return frustum.isInside(midPointX - HALF_CHUNK_SIZE, height, midPointZ + HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET) ||
         frustum.isInside(midPointX + HALF_CHUNK_SIZE, height, midPointZ + HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET) ||
         frustum.isInside(midPointX + HALF_CHUNK_SIZE, height, midPointZ - HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET) ||
         frustum.isInside(midPointX - HALF_CHUNK_SIZE, height, midPointZ - HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET);
}
