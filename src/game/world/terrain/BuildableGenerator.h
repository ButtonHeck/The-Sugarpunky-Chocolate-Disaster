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
  void setup(std::vector<std::vector<float>> &baseMap, std::vector<std::vector<float>> &hillsMap);
  GLuint& getNumInstances();
  GLuint& getSelectedTileVAO();
private:
  void setupAndBindBuffers(OpenglBuffer& buffers);
  void fillBufferData();
  OpenglBuffer selectedBuffers;
  unsigned int numInstances;
};

#endif // BUILDABLEGENERATOR_H
