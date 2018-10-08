#ifndef SETTINGS
#define SETTINGS
#include <GL/glew.h>
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
constexpr float FOV = 40.0f;

//world config
constexpr int WORLD_WIDTH = 384;
constexpr int WORLD_HEIGHT = 384;
constexpr int HALF_WORLD_WIDTH = WORLD_WIDTH / 2;
constexpr int HALF_WORLD_HEIGHT = WORLD_HEIGHT / 2;
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
constexpr unsigned int MESH_INDIRECT_BUFFER_UPDATE_FREQ = 2;
constexpr unsigned char QUAD_INDICES[6] = {0,1,2,2,3,0};

//textures configuration
constexpr bool HDR_ENABLED = true;
constexpr unsigned int MULTISAMPLES = 8;
constexpr float ANISOTROPY = 2.0f;

//constants for shadow calculations
constexpr float NEAR_PLANE_SHADOWING = 280.0f;
constexpr float FAR_PLANE_SHADOWING = 700.0f;
const glm::vec3 LIGHT_DIR_TO = glm::vec3(-0.2f, -0.16f, -0.08f);
const glm::vec3 LIGHT_POS = glm::vec3(328.0f, 254.0f, 311.0f);
const glm::mat4 LIGHT_PROJECTION = glm::ortho(-80.0f, 420.0f, -120.0f, 200.0f, NEAR_PLANE_SHADOWING, FAR_PLANE_SHADOWING);
const glm::mat4 LIGHT_VIEW = glm::lookAt(LIGHT_POS, LIGHT_POS + LIGHT_DIR_TO, glm::vec3(0.0f, 1.0f, 0.0f));
const glm::mat4 LIGHT_SPACE_MATRIX = LIGHT_PROJECTION * LIGHT_VIEW;
constexpr int DEPTH_MAP_TEXTURE_WIDTH = 16184;
constexpr int DEPTH_MAP_TEXTURE_HEIGHT = 10000;

//shader uniform strategy
constexpr bool UNIFORMS_NO_CACHE = false;
constexpr bool UNIFORMS_USE_CACHE = true;

#endif // SETTINGS

