#ifndef BASETERRAINCHUNK_H
#define BASETERRAINCHUNK_H
#include "Chunk.h"

class BaseTerrainChunk : public Chunk
{
public:
  BaseTerrainChunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom, unsigned int offset);
  void setInstanceOffset(unsigned int offset);
  unsigned int getInstanceOffset() const;
private:
  unsigned int instanceOffset;
};

#endif // BASETERRAINCHUNK_H
