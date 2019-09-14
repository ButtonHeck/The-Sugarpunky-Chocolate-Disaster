/*
 * Copyright 2019 Ilya Malgin
 * SceneSettings.h
 * This file is part of The Sugarpunky Chocolate Disaster project
 *
 * The Sugarpunky Chocolate Disaster project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Sugarpunky Chocolate Disaster project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * See <http://www.gnu.org/licenses/>
 *
 * Purpose: contains global constant scene settings
 * @version 0.1.0
 */

#pragma once

//scene config
constexpr int WORLD_WIDTH = 384;
constexpr int WORLD_HEIGHT = 384;
constexpr int HALF_WORLD_WIDTH = WORLD_WIDTH / 2;
constexpr float HALF_WORLD_WIDTH_F = static_cast<float>( HALF_WORLD_WIDTH );
constexpr int HALF_WORLD_HEIGHT = WORLD_HEIGHT / 2;
constexpr float HALF_WORLD_HEIGHT_F = static_cast<float>( HALF_WORLD_HEIGHT );
constexpr int NUM_TILES = WORLD_WIDTH * WORLD_HEIGHT;
constexpr float WATER_LEVEL = -1.0f;
constexpr int TILE_NO_RENDER_VALUE = -10;
constexpr int CHUNK_NO_RENDER_VALUE = -20;
constexpr float HILLS_OFFSET_Y = -0.2f;
constexpr unsigned int HILL_TILING_PER_TEXTURE_QUAD = 2;
constexpr float NORMAL_MAP_TILING_RECIPROCAL = 1.0f / 8.0f;
constexpr float UNDERWATER_TILE_Y = -4.0f;
constexpr int RIVER_WIDTH_BASE = 5;
constexpr unsigned int SHORE_SMOOTH_CYCLES = 5;
constexpr int PLANTS_DISTRIBUTION_FREQUENCY = 8;
constexpr float APPROXIMATE_LAND_PLANTS_CHUNK_HEIGHT = 2.0f;
constexpr float APPROXIMATE_GRASS_CHUNK_HEIGHT = 0.5f;
constexpr float APPROXIMATE_HILL_PLANTS_CHUNK_HEIGHT = 2.0f;

//scene update & rendering config
constexpr int CHUNK_SIZE = 4;
constexpr int NUM_CHUNKS = ( WORLD_WIDTH / CHUNK_SIZE ) * ( WORLD_HEIGHT / CHUNK_SIZE );
constexpr float HALF_CHUNK_SIZE = CHUNK_SIZE / 2.0f;
constexpr float FRUSTUM_CULLING_DISTANCE_OFFSET = CHUNK_SIZE;
constexpr float PLANET_MOVE_SPEED = 0.045f;
