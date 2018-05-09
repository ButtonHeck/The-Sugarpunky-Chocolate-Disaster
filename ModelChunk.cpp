#include "ModelChunk.h"

ModelChunk::ModelChunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom)
  :
    left(left),
    right(right),
    top(top),
    bottom(bottom)
{

}

void ModelChunk::setInstanceOffset(unsigned int instanceOffset)
{
  this->instanceOffset = instanceOffset;
}

void ModelChunk::setNumInstances(unsigned int numInstances)
{
  this->numInstances = numInstances;
}

unsigned int ModelChunk::getInstanceOffset()
{
  return instanceOffset;
}

unsigned int ModelChunk::getNumInstances()
{
  return numInstances;
}

bool ModelChunk::containsPoint(const glm::vec2 &position)
{
  if (position.y >= top && position.y < bottom)
    {
      if (position.x >= left && position.x < right)
        return true;
      else
        return false;
    }
  else
    return false;
}

unsigned int ModelChunk::getLeft()
{
  return left;
}

unsigned int ModelChunk::getRight()
{
  return right;
}

unsigned int ModelChunk::getTop()
{
  return top;
}

unsigned int ModelChunk::getBottom()
{
  return bottom;
}
