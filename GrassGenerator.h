#ifndef GRASSGENERATOR_H
#define GRASSGENERATOR_H
#include "BaseMapGenerator.h"
#include "HillsMapGenerator.h"
#include <random>
#include <vector>
#include <glm/glm.hpp>
#include "Settings.h"

class GrassGenerator
{
public:
  GrassGenerator(BaseMapGenerator& baseMapGenerator, HillsMapGenerator& hillsMapGenerator);
  void prepareMap();
  void fillBufferData();
  GLuint& getVAO(int i);
  unsigned int& getNumVAOS();
  unsigned int& getNumInstancesPerVao();
  void deleteGLObjects();
private:
  BaseMapGenerator& baseMapGenerator;
  HillsMapGenerator& hillsMapGenerator;
  unsigned int GRASS_PER_TILE = 1;
  unsigned int NUM_VAOS = 64;
  unsigned int num_instances_per_vao;
  std::default_random_engine randomizer;
  GLuint vbo, ebo;
  GLuint* modelVaos = new GLuint[NUM_VAOS];
  unsigned int numTiles = 0;
  std::vector<glm::mat4*> instanceModels;
};

#endif // GRASSGENERATOR_H
