#ifndef HILLSMAPGENERATOR_H
#define HILLSMAPGENERATOR_H
#include "MapGenerator.h"
#include "graphics/Shader.h"
#include <random>
#include <chrono>
#include <cstring>

class HillsMapGenerator : public MapGenerator
{
public:
  HillsMapGenerator(Shader& shader, std::vector<std::vector<float>>& waterMap);
  ~HillsMapGenerator();
  void prepareMap();
  void fillBufferData();
  void createTiles();
  float getMaxHeight() const;
  GLuint getCulledVAO() const;
  GLuint getTransformFeedback() const;
private:
  enum HILL_DENSITY
  {
    HILLS_THIN, HILLS_MEDIUM, HILLS_DENSE
  };
  GLuint culledVAO = 0, culledVBO = 0, TFBO = 0;
  Shader& hillsShader;
  float maxHeight = 0.0f;
  std::vector<std::vector<float>>& waterMap;
  std::vector<std::vector<glm::vec3>> normalMap;
  std::default_random_engine randomizer;
  void generateMap(int cycles, HILL_DENSITY density);
  bool hasWaterNearby(unsigned int x, unsigned int y, unsigned int radius);
  void compressMap(float thresholdPercent, float ratio);
  void updateMaxHeight();
  void removeMapPlateaus(float plateauHeight);
  void smoothMapHeightChunks(float baseWeight, float evenWeight, float diagonalWeight);
  void removeOrphanHills();
  bool isOrphanAt(int x, int y);
  void smoothMapSinks();
};

#endif // HILLSMAPGENERATOR_H
