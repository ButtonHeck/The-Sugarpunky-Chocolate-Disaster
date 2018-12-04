#ifndef SETTINGS
#define SETTINGS
#include <string>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unistd.h>

//benchmarking
#ifdef _DEBUG
#define BENCHMARK(benchmarkName, perFrame) BenchmarkTimer b(benchmarkName, perFrame);
#else
#define BENCHMARK(dont, care) //noop
#endif

//resource loading related
const std::string getResourcesDirectory();
const std::string RES_DIR = getResourcesDirectory() + "/res/";
const std::string SHADER_DIR = RES_DIR + "shaders/";
const std::string TEXTURES_DIR = RES_DIR + "textures/";
const std::string MODELS_DIR = RES_DIR + "models/";
const std::string FONT_DIR = RES_DIR + "fonts/";
const std::string SAVES_DIR = RES_DIR + "saves/";
constexpr bool INCLUDE_RES_DIR = true;

//view frustum config
constexpr float NEAR_PLANE = 0.1f;
constexpr float FAR_PLANE = 500.0f;
constexpr float FOV = 60.0f;

//world config
constexpr int WORLD_WIDTH = 384;
constexpr int WORLD_HEIGHT = 384;
constexpr int HALF_WORLD_WIDTH = WORLD_WIDTH / 2;
constexpr float HALF_WORLD_WIDTH_F = static_cast<float>(HALF_WORLD_WIDTH);
constexpr int HALF_WORLD_HEIGHT = WORLD_HEIGHT / 2;
constexpr float HALF_WORLD_HEIGHT_F = static_cast<float>(HALF_WORLD_HEIGHT);
constexpr int NUM_TILES = WORLD_WIDTH * WORLD_HEIGHT;
constexpr float WATER_LEVEL = -1.0f;
constexpr int TILE_NO_RENDER_VALUE = -10;
constexpr int CHUNK_NO_RENDER_VALUE = -20;
constexpr float HILLS_OFFSET_Y = -0.2f;
constexpr float SHORE_CLIP_LEVEL = -4.0f;
constexpr float UNDERWATER_TILE_YPOS = -4.0f;
constexpr int SHORE_SIZE_BASE = 5;

//world update & rendering config
constexpr int CHUNK_SIZE = 16;
constexpr float HALF_CHUNK_SIZE = CHUNK_SIZE / 2.0f;
constexpr unsigned int LOADING_DISTANCE_CHUNKS = 8;
constexpr unsigned int LOADING_DISTANCE_UNITS = CHUNK_SIZE * LOADING_DISTANCE_CHUNKS;
constexpr unsigned int LOADING_DISTANCE_UNITS_SQUARE = LOADING_DISTANCE_UNITS * LOADING_DISTANCE_UNITS;
constexpr float MODELS_FC_RADIUS = HALF_CHUNK_SIZE;
constexpr int MODELS_DISTRIBUTION_FREQ = 8;
constexpr unsigned char QUAD_INDICES[6] = {0,1,2,2,3,0};
constexpr unsigned int VERTICES_PER_TILE = 6;

//visual quality configuration
constexpr bool HDR_ENABLED = true;
constexpr unsigned int MULTISAMPLES = 4;
constexpr float ANISOTROPY = 2.0f;

//constants for lighting calculations
const glm::vec4 NIGHT_SKY_COLOR(0.02f, 0.0112f, 0.0088f, 1.0f);
const glm::vec4 DAY_SKY_COLOR(0.85f, 0.44f, 0.35f, 1.0f);
constexpr int DEPTH_MAP_TEXTURE_WIDTH = 4096;
constexpr int DEPTH_MAP_TEXTURE_HEIGHT = 4096;
const float SHADOW_NEAR_DISTANCE = 20.0f;
const float SHADOW_FAR_DISTANCE = 60.0f;

//shader uniform strategy
constexpr bool UNIFORMS_NO_CACHE = false;
constexpr bool UNIFORMS_USE_CACHE = true;

#endif // SETTINGS

