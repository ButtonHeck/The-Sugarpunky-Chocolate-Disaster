/*
 * Copyright 2019 Ilya Malgin
 * TextureUnits.h
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
 * Purpose: contains enumeration of all texture units
 * @version 0.1.0
 */

#pragma once

enum TEXTURE_UNIT
{
  TEX_LAND = 0,
  TEX_LAND_2,
  TEX_DIFFUSE_MIX_MAP,
  TEX_TERRAIN_NORMAL,
  TEX_WATER_NORMAL,
  TEX_WATER_SPECULAR,
  TEX_WATER_DUDV,
  TEX_HILL,
  TEX_HILL_2,
  TEX_HILL_SPECULAR,
  TEX_SHORE,
  TEX_SHORE_2,
  TEX_UNDERWATER_DIFFUSE,
  TEX_UNDERWATER_RELIEF,
  TEX_SKYBOX_HILLS_NEAR,
  TEX_SKYBOX_HILLS_NEAR_NORMAL,
  TEX_SKYBOX_HILLS_FAR,
  TEX_SKYBOX_HILLS_FAR_NORMAL,
  TEX_SKYBOX_CLOUDS,
  TEX_THE_SUN,
  TEX_SKYSPHERE_THE_SUN_AMBIENT_LIGHTING,
  TEX_SKYSPHERE_STARS,
  TEX_FRAME_MULTISAMPLED,
  TEX_FRAME,
  TEX_FRAME_HDR,
  TEX_FRAME_DEPTH,
  TEX_FRAME_VIGNETTE,
  TEX_FRAME_WATER_REFLECTION,
  TEX_FRAME_WATER_REFRACTION,
  TEX_FRAME_WATER_REFRACTION_DEPTH,
  TEX_DEPTH_MAP_SUN,
  TEX_FONT,
  TEXTURE_UNITS_SIZE
};
