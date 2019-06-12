#pragma once

#include "Chunk"

class LandChunk : public Chunk
{
public:
  LandChunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom, unsigned int offset, unsigned int instances);

  //instance offset
  void setInstanceOffset(unsigned int offset) noexcept;
  unsigned int getInstanceOffset() const noexcept;

  //num instances
  void setNumInstances(unsigned int instances) noexcept;
  unsigned int getNumInstances() const noexcept;

private:
  unsigned int instanceOffset;
  unsigned int numInstances;
};

inline void LandChunk::setInstanceOffset(unsigned int offset) noexcept
{
  this->instanceOffset = offset;
}

inline unsigned int LandChunk::getInstanceOffset() const noexcept
{
  return instanceOffset;
}

inline void LandChunk::setNumInstances(unsigned int instances) noexcept
{
  this->numInstances = instances;
}

inline unsigned int LandChunk::getNumInstances() const noexcept
{
  return numInstances;
}
