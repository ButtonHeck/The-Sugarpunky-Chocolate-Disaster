#include "ModelChunk.h"

ModelChunk::ModelChunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom)
  :
    Chunk(left, right, top, bottom)
{

}

void ModelChunk::setInstanceOffsetsVector(std::vector<unsigned int> &instanceOffsets)
{
  for (unsigned int i = 0; i < instanceOffsets.size(); i++)
    this->instanceOffsets.push_back(instanceOffsets[i]);
}

void ModelChunk::setInstanceOffset(unsigned int index, unsigned int offset)
{
  instanceOffsets[index] = offset;
}

void ModelChunk::setNumInstancesVector(std::vector<unsigned int> &numInstances)
{
  for (unsigned int i = 0; i < numInstances.size(); i++)
    this->numInstances.push_back(numInstances[i]);
}

void ModelChunk::setNumInstances(unsigned int index, unsigned int instances)
{
  numInstances[index] = instances;
}

unsigned int ModelChunk::getInstanceOffset(int index)
{
  return instanceOffsets[index];
}

std::vector<unsigned int> &ModelChunk::getInstanceOffsetVector()
{
  return instanceOffsets;
}

unsigned int ModelChunk::getNumInstances(int index)
{
  return numInstances[index];
}

std::vector<unsigned int> &ModelChunk::getNumInstancesVector()
{
  return numInstances;
}
