/*
 * Copyright 2019 Ilya Malgin
 * UnderwaterSurface.h
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
 * Purpose: contains declaration for UnderwaterSurface class
 * @version 0.1.0
 */

#pragma once

#include "BufferCollection"

/**
* @brief representation of the underwater surface tile.
* Responsible for initializing buffer collection and loading underwater tile data to GPU
*/
class UnderwaterSurface
{
public:
	UnderwaterSurface() noexcept;

private:
	friend class UnderwaterRenderer;

	BufferCollection basicGLBuffers;
};
