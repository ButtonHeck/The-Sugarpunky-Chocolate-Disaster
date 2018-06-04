#ifndef SETTINGS
#define SETTINGS
#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>

constexpr float NEAR_PLANE = 0.1f;
constexpr float FAR_PLANE = 500.0f;
constexpr float FOV = 40.0f;
constexpr float FOV_DOT_PRODUCT = std::cos(glm::radians(FOV));
constexpr int TILES_WIDTH = 384;
constexpr int TILES_HEIGHT = 384;
constexpr int HALF_TILES_WIDTH = TILES_WIDTH / 2;
constexpr int HALF_TILES_HEIGHT = TILES_HEIGHT / 2;
constexpr int NUM_TILES = TILES_WIDTH * TILES_HEIGHT;
constexpr float WATER_LEVEL = -0.75f;
constexpr int DENY_TILE_RENDER_VALUE = -10;
constexpr int DENY_CHUNK_RENDER_VALUE = -20;
constexpr float HILLS_OFFSET_Y = -0.4f;
constexpr float UNDERWATER_REMOVAL_LEVEL = -1.2f;
constexpr float UNDERWATER_BASE_TILE_HEIGHT = -1.2f;
constexpr int BASE_TERRAIN_CHUNK_SIZES[] = {128, 64, 32, 16, 8, 4};
constexpr int NUM_BASE_TERRAIN_CHUNKS = sizeof(BASE_TERRAIN_CHUNK_SIZES) / sizeof(int);
constexpr int SHORE_SIZE_BASE = 4;
constexpr unsigned int MODEL_CHUNK_SIZE = 16;
constexpr unsigned int CHUNK_LOADING_DISTANCE = 8;
constexpr unsigned int QUAD_INDICES[6] = {0,1,2,2,3,0};
constexpr bool HILLS_TEXTURE_MAPPING_SLOPE_CORRECTION = true;
constexpr bool BASE_TERRAIN_RANDOMIZE_SHORE_FORM = true;
const std::string PROJ_PATH("/home/ilya/QtProjects/MyOpenGL/TerrainGenerationTest");
const glm::vec3 LIGHT_DIR_TO = glm::vec3(-0.2f, -0.15f, -0.08f);

enum HILL_DENSITY
{
  THIN, MEDIUM, DENSE
};

enum TEXTURE_UNITS
{
  FLAT = 0,
  FLAT_2 = 1,
  FLAT_x2 = 2,
  FLAT_2_x2 = 3,
  NORMAL_MAP = 4,
  HILL = 5,
  HILL_2 = 6,
  HILL_SPECULAR = 7,
  SHORE = 8,
  SHORE_2 = 9,
  UNDERWATER = 10,
  WATER = 11,
  WATER_SPECULAR = 12,
  UNDERWATER_RELIEF = 13,
  SKYBOX = 14
};
#endif // SETTINGS

