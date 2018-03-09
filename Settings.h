#ifndef SETTINGS
#define SETTINGS
#include <string>

constexpr int SCR_WIDTH = 1440;
constexpr int SCR_HEIGHT = 900;
constexpr int TILES_WIDTH = 384;
constexpr int TILES_HEIGHT = 384;
constexpr int NUM_TILES = TILES_WIDTH * TILES_HEIGHT;
constexpr float WATER_LEVEL = -0.75f;
constexpr int DENY_TILE_RENDER_VALUE = -10;
constexpr int DENY_CHUNK_RENDER_VALUE = -20;
constexpr float UNDERWATER_REMOVAL_LEVEL = -1.2f;
constexpr float UNDERWATER_BASE_TILE_HEIGHT = -1.3f;
constexpr int BASE_TERRAIN_CHUNK_SIZE = 64;
constexpr int BASE_TERRAIN_CHUNK_SIZE2 = 32;
constexpr int BASE_TERRAIN_CHUNK_SIZE3 = 24;
constexpr int BASE_TERRAIN_CHUNK_SIZE4 = 16;
constexpr int BASE_TERRAIN_CHUNK_SIZE5 = 8;
constexpr unsigned int SHORE_SIZE_BASE = 5;
const std::string PROJ_PATH("/home/ilya/QtProjects/MyOpenGL/TerrainGenerationTest");

enum HILL_DENSITY
{
  THIN, MEDIUM, DENSE
};

/*FMG - aka Fucking Magic Value
* this one is used for the arrayLength of @baseTerrainHeightOffsets(main.cpp)
* without it added to the arrayLength we could see a tile that goes straight to the
* hell/heaven at the very bottom-right corner...
* because shit happens. If you read this, please, do something.
*/
constexpr int FMG = 1;

#endif // SETTINGS

