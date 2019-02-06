#ifndef LANDCHUNK_H
#define LANDCHUNK_H
#include "game/world/Chunk.h"
#include "graphics/Frustum.h"

class LandChunk : public Chunk
{
public:
  LandChunk(unsigned int left,
            unsigned int right,
            unsigned int top,
            unsigned int bottom,
            unsigned int offset,
            unsigned int instances);
  bool isInsideFrustum(const Frustum& frustum) const;

  //instance offset
  void setInstanceOffset(unsigned int offset);
  unsigned int getInstanceOffset() const;

  //num instances
  void setNumInstances(unsigned int instances);
  unsigned int getNumInstances() const;

private:
  unsigned int instanceOffset;
  unsigned int numInstances;
};

inline void LandChunk::setInstanceOffset(unsigned int offset)
{
  this->instanceOffset = offset;
}

inline unsigned int LandChunk::getInstanceOffset() const
{
  return instanceOffset;
}

inline void LandChunk::setNumInstances(unsigned int instances)
{
  this->numInstances = instances;
}

inline unsigned int LandChunk::getNumInstances() const
{
  return numInstances;
}

#endif // LANDCHUNK_H
