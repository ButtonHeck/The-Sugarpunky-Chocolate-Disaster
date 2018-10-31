#ifndef BUILDABLEGENERATOR_H
#define BUILDABLEGENERATOR_H
#include <vector>
#include <memory>
#include "game/world/terrain/Generator.h"
#include "util/Settings.h"

class BuildableGenerator : public Generator
{
public:
  BuildableGenerator();
  virtual ~BuildableGenerator() = default;
  void setup(map2D_f &landMap, map2D_f &hillsMap);

private:
  friend class BuildableRenderer;

  void setupAndBindBuffers(OpenglBuffer& buffers);
  void fillBufferData();

  OpenglBuffer selectedBuffers;
  unsigned int numInstances;
};

#endif // BUILDABLEGENERATOR_H
