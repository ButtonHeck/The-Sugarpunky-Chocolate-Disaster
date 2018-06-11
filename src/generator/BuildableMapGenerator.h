#ifndef BUILDABLEMAPGENERATOR_H
#define BUILDABLEMAPGENERATOR_H
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MapGenerator.h"
#include "src/generator/BaseMapGenerator.h"
#include "src/generator/WaterMapGenerator.h"
#include "src/generator/HillsMapGenerator.h"
#include "src/game/Settings.h"

class BuildableMapGenerator : public MapGenerator
{
public:
  BuildableMapGenerator(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap);
  ~BuildableMapGenerator();
  void prepareMap();
  void fillBufferData();
  GLuint& getNumInstances();
  GLuint& getSelectedTileVAO();
  void deleteGLObjects();
private:
  std::vector<std::vector<float>>& baseMap;
  std::vector<std::vector<float>>& hillMap;
  GLuint modelVbo;
  GLuint selectedVAO, selectedVBO, selectedEBO;
  unsigned int num_instances;
};

#endif // BUILDABLEMAPGENERATOR_H
