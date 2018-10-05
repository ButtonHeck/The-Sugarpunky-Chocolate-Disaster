#ifndef BUILDABLEMAPGENERATOR_H
#define BUILDABLEMAPGENERATOR_H
#include <vector>
#include <memory>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "generator/BaseMapGenerator.h"
#include "generator/HillsMapGenerator.h"
#include "game/Settings.h"

class BuildableMapGenerator : public MapGenerator
{
public:
  BuildableMapGenerator(std::shared_ptr<BaseMapGenerator>& baseMapGenerator,
                        std::shared_ptr<HillsMapGenerator>& hillsGenerator);
  ~BuildableMapGenerator();
  void setup(std::shared_ptr<BaseMapGenerator> &baseMapGenerator,
             std::shared_ptr<HillsMapGenerator> &hillsGenerator);
  GLuint& getNumInstances();
  GLuint& getSelectedTileVAO();
  void deleteGLObjects();
private:
  void fillBufferData();
  std::shared_ptr<BaseMapGenerator>& baseMapGenerator;
  std::shared_ptr<HillsMapGenerator>& hillsGenerator;
  GLuint modelVbo;
  GLuint selectedVAO, selectedVBO, selectedEBO;
  unsigned int num_instances;
};

#endif // BUILDABLEMAPGENERATOR_H
