#include "game/world/terrain/land/LandChunk.h"

LandChunk::LandChunk(unsigned int left,
                     unsigned int right,
                     unsigned int top,
                     unsigned int bottom,
                     unsigned int offset,
                     unsigned int instances)
  :
    Chunk(left, right, top, bottom),
    instanceOffset(offset),
    numInstances(instances)
{}

bool LandChunk::isInsideFrustum(const Frustum &frustum) const
{
  return frustum.isInside(midPointX - HALF_CHUNK_SIZE, 0.0f, midPointZ + HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET) ||
         frustum.isInside(midPointX + HALF_CHUNK_SIZE, 0.0f, midPointZ + HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET) ||
         frustum.isInside(midPointX + HALF_CHUNK_SIZE, 0.0f, midPointZ - HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET) ||
         frustum.isInside(midPointX - HALF_CHUNK_SIZE, 0.0f, midPointZ - HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET);
}
