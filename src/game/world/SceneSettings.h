#ifndef SCENESETTINGS_H
#define SCENESETTINGS_H

//scene config
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
constexpr unsigned int HILL_TILING_PER_TEXTURE_QUAD = 2;
constexpr float NORMAL_MAP_TILING_RECIPROCAL = 1.0f / 8.0f;
constexpr float UNDERWATER_TILE_YPOS = -4.0f;
constexpr int RIVER_WIDTH_DEFAULT = 5;
constexpr unsigned int SHORE_SMOOTH_CYCLES = 5;
constexpr int PLANTS_DISTRIBUTION_FREQUENCY = 8;

//scene update & rendering config
constexpr int CHUNK_SIZE = 4;
constexpr float HALF_CHUNK_SIZE = CHUNK_SIZE / 2.0f;
constexpr float FRUSTUM_CULLING_DISTANCE_OFFSET = CHUNK_SIZE;
constexpr float PLANET_MOVE_SPEED = 0.045f;

#endif // SCENESETTINGS_H
