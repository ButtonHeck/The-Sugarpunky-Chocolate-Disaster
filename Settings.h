#ifndef SETTINGS
#define SETTINGS
#include <string>
#include <glm/glm.hpp>

constexpr int TILES_WIDTH = 384;
constexpr int TILES_HEIGHT = 384;
constexpr int NUM_TILES = TILES_WIDTH * TILES_HEIGHT;
constexpr float WATER_LEVEL = -0.75f;
constexpr int DENY_TILE_RENDER_VALUE = -10;
constexpr int DENY_CHUNK_RENDER_VALUE = -20;
constexpr float UNDERWATER_REMOVAL_LEVEL = -1.2f;
constexpr float UNDERWATER_BASE_TILE_HEIGHT = -1.2f;
constexpr int BASE_TERRAIN_CHUNK_SIZES[] = {128, 64, 32, 16, 8, 4};
constexpr int NUM_BASE_TERRAIN_CHUNKS = sizeof(BASE_TERRAIN_CHUNK_SIZES) / sizeof(int);
constexpr int SHORE_SIZE_BASE = 4;
constexpr unsigned int QUAD_INDICES[6] = {0,1,2,2,3,0};
const std::string PROJ_PATH("/home/ilya/QtProjects/MyOpenGL/TerrainGenerationTest");
const glm::vec3 LIGHT_DIR_TO = glm::vec3(-0.2f, -0.15f, -0.08f);

enum HILL_DENSITY
{
  THIN, MEDIUM, DENSE
};
#endif // SETTINGS

