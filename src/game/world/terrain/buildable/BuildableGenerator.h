#ifndef BUILDABLEGENERATOR_H
#define BUILDABLEGENERATOR_H
#include <memory>
#include "../../src/game/world/terrain/Generator.h"

class BuildableGenerator : public Generator
{
public:
  BuildableGenerator();
  virtual ~BuildableGenerator() = default;
  void setup(const map2D_f &landMap, const map2D_f &hillsMap);

private:
  friend class BuildableRenderer;
  const unsigned int UPPER_LEFT_CORNER_START_Y = 2;
  const unsigned int UPPER_LEFT_CORNER_START_X = 1;

  void setupAndBindBuffers(BufferCollection& buffers);
  void fillBufferData();

  BufferCollection selectedBuffers;
};

#endif // BUILDABLEGENERATOR_H
