#ifndef BUILDABLEGENERATOR_H
#define BUILDABLEGENERATOR_H
#include <vector>
#include <memory>
#include "game/world/terrain/LandGenerator.h"
#include "game/world/terrain/HillsGenerator.h"
#include "util/Settings.h"

class BuildableGenerator : public Generator
{
public:
  BuildableGenerator(std::shared_ptr<LandGenerator>& baseMapGenerator,
                        std::shared_ptr<HillsGenerator>& hillsGenerator);
  virtual ~BuildableGenerator() = default;
  void setup(std::shared_ptr<LandGenerator> &baseMapGenerator,
             std::shared_ptr<HillsGenerator> &hillsGenerator);
  GLuint& getNumInstances();
  GLuint& getSelectedTileVAO();
private:
  void setupAndBindBuffers(OpenglBuffer& buffers);
  void fillBufferData();
  std::shared_ptr<LandGenerator>& baseMapGenerator;
  std::shared_ptr<HillsGenerator>& hillsGenerator;
  OpenglBuffer selectedBuffers;
  unsigned int numInstances;
};

#endif // BUILDABLEGENERATOR_H
