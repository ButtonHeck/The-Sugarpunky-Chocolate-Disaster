#include "ModelChunk.h"

ModelChunk::ModelChunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom)
  :
    left(left),
    right(right),
    top(top),
    bottom(bottom)
{

}

void ModelChunk::setInstanceOffsets(std::vector<unsigned int> &instanceOffsets)
{
  for (unsigned int i = 0; i < instanceOffsets.size(); i++)
    this->instanceOffsets.push_back(instanceOffsets[i]);
}

void ModelChunk::setNumInstances(std::vector<unsigned int> &numInstances)
{
  for (unsigned int i = 0; i < numInstances.size(); i++)
    this->numInstances.push_back(numInstances[i]);
}

unsigned int ModelChunk::getInstanceOffset(int index)
{
  return instanceOffsets[index];
}

unsigned int ModelChunk::getNumInstances(int index)
{
  return numInstances[index];
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
