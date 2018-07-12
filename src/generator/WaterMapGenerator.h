#ifndef WATERMAPGENERATOR_H
#define WATERMAPGENERATOR_H
#include "MapGenerator.h"
#include <GLFW/glfw3.h>
#include "game/Options.h"

class WaterMapGenerator : public MapGenerator
{
public:
  WaterMapGenerator();
  ~WaterMapGenerator();
  void prepareMap();
  void postPrepareMap();
  void fillBufferData();
  void bufferVertices();
  GLfloat* getHeightOffsets();
  void updateAnimationFrame(Options& options);
  constexpr static int WATER_HEIGHT_OFFSETS_SIZE = NUM_TILES + TILES_WIDTH * 2;
private:
  size_t numVertices;
  GLfloat* vertices;
  GLfloat waterHeightOffsets[WATER_HEIGHT_OFFSETS_SIZE]; //a bit overhead, because all we use is the part where we have water...
  //also, we don't have to init waterHeightOffsets, because we update its data every frame
  void generateMap(int shoreSizeBase, float waterLevel, unsigned int &numWaterTiles);
  void addWaterNearbyBaseTerrain();
  void fillSharpTerrainWithWater();
  void liftWaterLevel(float liftValue);
  std::vector<std::vector<float>> postProcessMap;
};

#endif // WATERMAPGENERATOR_H
