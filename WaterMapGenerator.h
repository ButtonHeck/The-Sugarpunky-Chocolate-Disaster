#ifndef WATERMAPGENERATOR_H
#define WATERMAPGENERATOR_H
#include "MapGenerator.h"
#include <GLFW/glfw3.h>
#include <GL/glew.h>

class WaterMapGenerator : public MapGenerator
{
public:
  WaterMapGenerator();
  void prepareMap();
  void fillWaterBufferData();
  GLfloat* getHeightOffsets();
  constexpr static int WATER_HEIGHT_OFFSETS_SIZE = NUM_TILES + TILES_WIDTH * 2;
private:
  GLfloat waterHeightOffsets[WATER_HEIGHT_OFFSETS_SIZE]; //a bit overhead, because all we use is the part where we have water...
  //also, we don't have to init waterHeightOffsets, because we update its data every frame
  void generateWaterMap(unsigned int shoreSizeBase, float waterLevel, unsigned int &numWaterTiles);
  void addWaterNearbyBaseTerrain();
  void fillSharpTerrainWithWater();
  void liftWaterLevel(float liftValue);
};

#endif // WATERMAPGENERATOR_H
