/*
 * Copyright 2019 Ilya Malgin
 * Options.cpp
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
 * Purpose: contains definitions for Options class
 * @version 0.1.0
 */

#include "Options"

/**
* @brief plain ctor, initializes all options to appropriate default values
*/
Options::Options() noexcept
{
	options[OPT_DRAW_TREES] = true;
	options[OPT_ANIMATE_WATER] = true;
	options[OPT_DRAW_DEBUG_TEXT] = true;
	options[OPT_RECREATE_TERRAIN_REQUEST] = false;
	options[OPT_SAVE_REQUEST] = false;
	options[OPT_LOAD_REQUEST] = false;
	options[OPT_SHOW_CURSOR] = false;
	options[OPT_DRAW_BUILDABLE] = false;
	options[OPT_HILLS_CULLING] = true;
	options[OPT_USE_OCCLUSION_CULLING] = false;
	options[OPT_WATER_CULLING] = false;
	options[OPT_USE_MULTISAMPLING] = true;
	options[OPT_USE_SHADOWS] = true;
	options[OPT_DRAW_LAND] = true;
	options[OPT_DRAW_WATER] = true;
	options[OPT_DRAW_HILLS] = true;
	options[OPT_MODELS_LAND_BLENDING] = true;
	options[OPT_MODELS_PHONG_SHADING] = true;
	options[OPT_POLYGON_LINE] = false;
	options[OPT_DEBUG_RENDER] = false;
	options[OPT_SHADOW_CAMERA_FIXED] = false;
	options[OPT_USE_SHADOW_CAMERA_MATRIX] = false;
	options[OPT_CSM_VISUALIZATION] = false;
	options[OPT_FRUSTUM_VISUALIZATION] = true;
	options[OPT_EXPECTED_VOLUME_VISUALIZATION] = true;
	options[OPT_ACTUAL_VOLUME_VISUALIZATION] = true;
	options[OPT_USE_DOF] = true;
	options[OPT_USE_VIGNETTE] = true;
	options[OPT_GRASS_SHADOW] = false;
	options[OPT_SHOW_VRAM_AVAILABLE] = false;
}

/**
* @brief utility operator overloading that just returns bool value for a given option
* @param option option to check
*/
std::bitset<OPTIONS_COUNT>::reference Options::operator[]( OPTION option )
{
	return options[option];
}

/**
* @brief switches option state
* @param option option to switch
*/
void Options::toggle( OPTION option )
{
	options.flip( option );
}
