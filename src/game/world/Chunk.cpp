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
