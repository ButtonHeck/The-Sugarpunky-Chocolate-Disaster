/*
 * Copyright 2019 Ilya Malgin
 * OptionsUnits.h
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
 * Purpose: contains enumeration of all options
 * @version 0.1.0
 */

#pragma once

enum OPTION
{
	OPT_DRAW_TREES = 0,
	OPT_ANIMATE_WATER,
	OPT_DRAW_DEBUG_TEXT,
	OPT_RECREATE_TERRAIN_REQUEST,
	OPT_SAVE_REQUEST,
	OPT_LOAD_REQUEST,
	OPT_SHOW_CURSOR,
	OPT_DRAW_BUILDABLE,
	OPT_HILLS_CULLING,
	OPT_WATER_CULLING,
	OPT_USE_MULTISAMPLING,
	OPT_USE_SHADOWS,
	OPT_DRAW_LAND,
	OPT_DRAW_WATER,
	OPT_DRAW_HILLS,
	OPT_MODELS_LAND_BLENDING,
	OPT_MODELS_PHONG_SHADING,
	OPT_POLYGON_LINE,
	OPT_DEBUG_RENDER,
	OPT_SHADOW_CAMERA_FIXED,
	OPT_USE_SHADOW_CAMERA_MATRIX,
	OPT_CSM_VISUALIZATION,
	OPT_FRUSTUM_VISUALIZATION,
	OPT_EXPECTED_VOLUME_VISUALIZATION,
	OPT_ACTUAL_VOLUME_VISUALIZATION,
	OPT_USE_DOF,
	OPT_USE_VIGNETTE,
	OPT_GRASS_SHADOW,
	OPT_SHOW_VRAM_AVAILABLE,
	OPTIONS_COUNT
};
