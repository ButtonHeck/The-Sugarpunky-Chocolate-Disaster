#ifndef CELLCHUNK_H
#define CELLCHUNK_H
#include "BaseTerrainChunk.h"

class CellChunk : public BaseTerrainChunk
{
public:
  CellChunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom, unsigned int offset, unsigned int instances);
  void setNumInstances(unsigned int instances);
  unsigned int getNumInstances() const;
private:
  unsigned int numInstances;
};

#endif // CELLCHUNK_H
