/*
 * Copyright 2019 Ilya Malgin
 * GraphicsConstants.h
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
 * Purpose: contains global constant graphics settings
 * @version 0.1.0
 */

#pragma once

#include <glm/vec4.hpp>

const glm::vec4 NIGHT_SKY_COLOR( 0.02f, 0.0112f, 0.0088f, 1.0f );
const glm::vec4 DAY_SKY_COLOR( 0.75f, 0.38f, 0.3f, 1.0f );
constexpr unsigned int NUM_SHADOW_LAYERS = 3;
