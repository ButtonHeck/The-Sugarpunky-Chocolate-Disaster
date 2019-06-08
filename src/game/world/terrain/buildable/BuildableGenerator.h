#pragma once

#include "Generator"

#include <memory>

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
