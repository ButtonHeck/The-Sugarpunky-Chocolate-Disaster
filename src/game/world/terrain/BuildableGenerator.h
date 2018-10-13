#ifndef BUILDABLEGENERATOR_H
#define BUILDABLEGENERATOR_H
#include <vector>
#include <memory>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "game/world/terrain/LandGenerator.h"
#include "game/world/terrain/HillsGenerator.h"
#include "game/Settings.h"

class BuildableGenerator : public Generator
{
public:
  BuildableGenerator(std::shared_ptr<LandGenerator>& baseMapGenerator,
                        std::shared_ptr<HillsGenerator>& hillsGenerator);
  virtual ~BuildableGenerator();
  void setup(std::shared_ptr<LandGenerator> &baseMapGenerator,
             std::shared_ptr<HillsGenerator> &hillsGenerator);
  GLuint& getNumInstances();
  GLuint& getSelectedTileVAO();
private:
  void setupVAO(GLuint& vao, GLuint& vbo, GLuint& ebo);
  void fillBufferData();
  std::shared_ptr<LandGenerator>& baseMapGenerator;
  std::shared_ptr<HillsGenerator>& hillsGenerator;
  GLuint modelVbo;
  GLuint selectedVAO, selectedVBO, selectedEBO;
  unsigned int numInstances;
};

#endif // BUILDABLEGENERATOR_H
