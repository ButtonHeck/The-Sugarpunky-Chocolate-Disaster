#ifndef CELLCHUNK_H
#define CELLCHUNK_H
#include "src/chunk/BaseTerrainChunk.h"

class CellChunk : public BaseTerrainChunk
{
public:
  CellChunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom, unsigned int offset, unsigned int instances);
  void setNumInstances(unsigned int instances);
  unsigned int getNumInstances() const;
private:
  unsigned int numInstances;
};

inline void CellChunk::setNumInstances(unsigned int instances)
{
  this->numInstances = instances;
}

inline unsigned int CellChunk::getNumInstances() const
{
  return numInstances;
}

#endif // CELLCHUNK_H
