#ifndef HILLSMAPGENERATOR_H
#define HILLSMAPGENERATOR_H
#include "MapGenerator.h"
#include <random>
#include "src/graphics/Camera.h"

class HillsMapGenerator : public MapGenerator
{
public:
  HillsMapGenerator(std::vector<std::vector<float>>& waterMap);
  ~HillsMapGenerator();
  void prepareMap();
  void fillBufferData(bool textureSlopeCorrection);
  void createTiles();
private:
  std::vector<std::vector<float>>& waterMap;
  void generateMap(int cycles, float* max_height, HILL_DENSITY density);
  bool hasWaterNearby(unsigned int x, unsigned int y, unsigned int radius);
  void compressMap(float threshold_percent, float* limit, float ratio);
  void removeMapPlateaus(float plateauHeight);
  void smoothMapHeightChunks(float baseWeight, float evenWeight, float diagonalWeight);
  void removeOrphanHills();
  bool isOrphanAt(int x, int y);
  void smoothMapSinks();
};

#endif // HILLSMAPGENERATOR_H
