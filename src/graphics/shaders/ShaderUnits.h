/*
 * Copyright 2019 Ilya Malgin
 * ShaderUnits.h
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
 * Purpose: contains enumeration of all shader program units
 * @version 0.1.0
 */

#pragma once

enum SHADER_UNIT
{
	SHADER_HILLS_CULLING = 0,
	SHADER_HILLS,
	SHADER_HILLS_NORMALS,
	SHADER_SHORE,
	SHADER_SHORE_NORMALS,
	SHADER_UNDERWATER,
	SHADER_LAND,
	SHADER_WATER_CULLING,
	SHADER_WATER,
	SHADER_WATER_NORMALS,
	SHADER_SKYBOX,
	SHADER_SKYSPHERE,
	SHADER_SUN,
	SHADER_MODELS_GOURAUD,
	SHADER_MODELS_PHONG,
	SHADER_FONT,
	SHADER_COORDINATE_SYSTEM,
	SHADER_BUILDABLE,
	SHADER_SELECTED,
	SHADER_MS_TO_DEFAULT,
	SHADER_SHADOW_TERRAIN,
	SHADER_SHADOW_MODELS,
	SHADER_FRUSTUM,
	SHADER_LENS_FLARE
};
