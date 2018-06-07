#include "BaseTerrainChunk.h"

BaseTerrainChunk::BaseTerrainChunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom, unsigned int offset)
  :
    Chunk(left, right, top, bottom),
    instanceOffset(offset)
{

}

void BaseTerrainChunk::setInstanceOffset(unsigned int offset)
{
  this->instanceOffset = offset;
}

unsigned int BaseTerrainChunk::getInstanceOffset() const
{
  return instanceOffset;
}
