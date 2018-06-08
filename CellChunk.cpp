#include "CellChunk.h"

CellChunk::CellChunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom, unsigned int offset, unsigned int instances)
  :
    BaseTerrainChunk(left, right, top, bottom, offset),
    numInstances(instances)
{

}
