#include "Chunk.h"

Chunk::Chunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom)
  :
    left(left),
    right(right),
    top(top),
    bottom(bottom)
{
  midPointX = -HALF_TILES_WIDTH + (right - left) / 2.0f + left;
  midPointY = -HALF_TILES_HEIGHT + (bottom - top) / 2.0f + top;
}

bool Chunk::containsPoint(float x, float y) const
{
  if (y >= top && y < bottom)
    {
      if (x >= left && x < right)
        return true;
      else
        return false;
    }
  else
    return false;
}

unsigned int Chunk::getLeft() const
{
  return left;
}

unsigned int Chunk::getRight() const
{
  return right;
}

unsigned int Chunk::getTop() const
{
  return top;
}

unsigned int Chunk::getBottom() const
{
  return bottom;
}

glm::vec2 Chunk::getMidPoint() const
{
  return glm::vec2(midPointX, midPointY);
}
