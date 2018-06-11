#ifndef TERRAINCHUNK_H
#define TERRAINCHUNK_H
#include "Chunk.h"

class TerrainChunk : public Chunk
{
public:
  TerrainChunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom, unsigned int offset, unsigned int instances);
  void setInstanceOffset(unsigned int offset);
  unsigned int getInstanceOffset() const;
  void setNumInstances(unsigned int instances);
  unsigned int getNumInstances() const;
private:
  unsigned int instanceOffset;
  unsigned int numInstances;
};

inline void TerrainChunk::setInstanceOffset(unsigned int offset)
{
  this->instanceOffset = offset;
}

inline unsigned int TerrainChunk::getInstanceOffset() const
{
  return instanceOffset;
}

inline void TerrainChunk::setNumInstances(unsigned int instances)
{
  this->numInstances = instances;
}

inline unsigned int TerrainChunk::getNumInstances() const
{
  return numInstances;
}

#endif // TERRAINCHUNK_H
