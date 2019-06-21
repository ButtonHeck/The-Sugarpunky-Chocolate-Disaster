/*
 * Copyright 2019 Ilya Malgin
 * TerrainTile.cpp
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
 * Purpose: contains definitions for TerrainTile struct
 * @version 0.1.0
 */

#include "TerrainTile"

/**
* @brief plain ctor
*/
TerrainTile::TerrainTile(int x, int y, float lowLeft, float lowRight, float upperRight, float upperLeft) noexcept
  :
    mapX(x),
    mapY(y),
    lowLeft(lowLeft),
    lowRight(lowRight),
    upperRight(upperRight),
    upperLeft(upperLeft)
{}
