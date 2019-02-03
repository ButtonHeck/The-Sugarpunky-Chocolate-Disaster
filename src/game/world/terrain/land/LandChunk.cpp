#include "game/world/terrain/land/LandChunk.h"

LandChunk::LandChunk(unsigned int left,
                     unsigned int right,
                     unsigned int top,
                     unsigned int bottom,
                     unsigned int offset,
                     unsigned int instances)
  :
    Chunk(left, right, top, bottom),
    instanceOffset(offset),
    numInstances(instances)
{}
