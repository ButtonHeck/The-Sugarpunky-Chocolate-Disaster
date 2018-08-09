#ifndef MODELCHUNK_H
#define MODELCHUNK_H
#include <vector>
#include "Chunk.h"

class ModelChunk : public Chunk
{
public:
  ModelChunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom);
  void setInstanceOffsetsVector(std::vector<unsigned int>& instanceOffsets);
  void setInstanceOffset(unsigned int index, unsigned int offset);
  void setNumInstancesVector(std::vector<unsigned int>& numInstances);
  void setNumInstances(unsigned int index, unsigned int instances);
  unsigned int getInstanceOffset(int index) const;
  std::vector<unsigned int>& getInstanceOffsetVector();
  unsigned int getNumInstances(int index) const;
  std::vector<unsigned int>& getNumInstancesVector();
private:
  std::vector<unsigned int> instanceOffsets;
  std::vector<unsigned int> numInstances;
};

inline void ModelChunk::setInstanceOffsetsVector(std::vector<unsigned int> &instanceOffsets)
{
  for (unsigned int i = 0; i < instanceOffsets.size(); i++)
    this->instanceOffsets.emplace_back(instanceOffsets[i]);
}

inline void ModelChunk::setInstanceOffset(unsigned int index, unsigned int offset)
{
  instanceOffsets[index] = offset;
}

inline void ModelChunk::setNumInstancesVector(std::vector<unsigned int> &numInstances)
{
  for (unsigned int i = 0; i < numInstances.size(); i++)
    this->numInstances.emplace_back(numInstances[i]);
}

inline void ModelChunk::setNumInstances(unsigned int index, unsigned int instances)
{
  numInstances[index] = instances;
}

inline unsigned int ModelChunk::getInstanceOffset(int index) const
{
  return instanceOffsets[index];
}

inline std::vector<unsigned int> &ModelChunk::getInstanceOffsetVector()
{
  return instanceOffsets;
}

inline unsigned int ModelChunk::getNumInstances(int index) const
{
  return numInstances[index];
}

inline std::vector<unsigned int> &ModelChunk::getNumInstancesVector()
{
  return numInstances;
}

#endif // MODELCHUNK_H
