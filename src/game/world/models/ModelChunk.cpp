#include "game/world/models/ModelChunk.h"

ModelChunk::ModelChunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom)
  :
    Chunk(left, right, top, bottom)
{}

bool ModelChunk::isInsideFrustum(const Frustum &frustum) const
{
  BENCHMARK("ModelChunk: isInsideFrustum", true);
  const float CULLING_Y_APPROX = 20.0f;
  return frustum.isInsideXZ(midPointX - HALF_CHUNK_SIZE, midPointZ + HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET) ||
         frustum.isInsideXZ(midPointX + HALF_CHUNK_SIZE, midPointZ + HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET) ||
         frustum.isInsideXZ(midPointX + HALF_CHUNK_SIZE, midPointZ - HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET) ||
         frustum.isInsideXZ(midPointX - HALF_CHUNK_SIZE, midPointZ - HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET) ||
         frustum.isInside(midPointX - HALF_CHUNK_SIZE, CULLING_Y_APPROX, midPointZ + HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET) ||
         frustum.isInside(midPointX + HALF_CHUNK_SIZE, CULLING_Y_APPROX, midPointZ + HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET) ||
         frustum.isInside(midPointX + HALF_CHUNK_SIZE, CULLING_Y_APPROX, midPointZ - HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET) ||
         frustum.isInside(midPointX - HALF_CHUNK_SIZE, CULLING_Y_APPROX, midPointZ - HALF_CHUNK_SIZE, FRUSTUM_CULLING_DISTANCE_OFFSET);
}
