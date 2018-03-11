#ifndef HILLSMAPGENERATOR_H
#define HILLSMAPGENERATOR_H
#include "MapGenerator.h"
#include <random>

class HillsMapGenerator : public MapGenerator
{
public:
  HillsMapGenerator(std::vector<std::vector<float>>& waterMap);
  void prepareMap();
  void fillHillsBuffersData();
  void generateHillMap(int cycles, float* max_height, HILL_DENSITY density);
  bool hasWaterNearby(unsigned int x, unsigned int y, unsigned int radius);
  void compressHeightHillMap(float threshold_percent, float* limit, float ratio);
  void removeHillMapPlateaus(float plateauHeight);
  void smoothHillMapHeightChunks(float baseWeight, float evenWeight, float diagonalWeight);
  void removeOrphanHills();
  bool isOrphanAt(int x, int y);
  void smoothHillMapSinks();
  void draw(Shader& shader);
  void deleteGLObjects();
  GLuint& getVAO();
  GLuint& getVBO();
  GLuint& getEBO();
private:
  GLuint vao, vbo, ebo;
  std::vector<std::vector<float>>& waterMap;
};

#endif // HILLSMAPGENERATOR_H
