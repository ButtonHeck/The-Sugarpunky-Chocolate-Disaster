#include "chunk/TerrainChunk.h"

TerrainChunk::TerrainChunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom, unsigned int offset, unsigned int instances)
  :
    Chunk(left, right, top, bottom),
    instanceOffset(offset),
    numInstances(instances)
{

}
