/*
 * Copyright 2019 Ilya Malgin
 * ModelIndirectBufferTypes.h
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
 * Purpose: contains enumeration of possible model indirect buffer usage types
 * @version 0.1.0
 */

#pragma once

enum MODEL_INDIRECT_BUFFER_TYPE
{
	PLAIN_ONSCREEN,
	DEPTHMAP_OFFSCREEN,
	REFLECTION_ONSCREEN
};