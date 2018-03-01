#ifndef SETTINGS
#define SETTINGS
#include <string>

constexpr int SCR_WIDTH = 1440;
constexpr int SCR_HEIGHT = 900;
constexpr int TILES_WIDTH = 512;
constexpr int TILES_HEIGHT = 512;
constexpr int NUM_TILES = TILES_WIDTH * TILES_HEIGHT;
const std::string PROJ_PATH("/home/ilya/QtProjects/MyOpenGL/TerrainGenerationTest");

enum HILL_DENSITY
{
  THIN, MEDIUM, DENSE
};

#endif // SETTINGS

