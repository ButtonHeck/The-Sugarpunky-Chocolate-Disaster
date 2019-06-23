/*
 * Copyright 2019 Ilya Malgin
 * typeAliases.h
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
 * Purpose: contains aliases for some commonly used types
 * @version 0.1.0
 */

#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

using map2D_f = std::vector<std::vector<float>>;
using map2D_i = std::vector<std::vector<int>>;
using map2D_vec3 = std::vector<std::vector<glm::vec3>>;
using map2D_mat4 = std::vector<std::vector<glm::mat4>>;
template<typename T> using map2D_template = std::vector<std::vector<T>>;
template<typename T> using vec2D_template = std::vector<std::vector<T>>;
