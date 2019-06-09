/*
 * Copyright 2019 Ilya Malgin
 * SkysphereShader.h
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
 * Purpose: contains declaration for SkysphereShader class
 * @version 0.1.0
 */

#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Shader;

/**
 * @brief Personal shader manager for skysphere related code module.
 * Responsible for updating non-constant uniform variables
 * and switching between multiple skysphere types
 */
class SkysphereShader
{
public:
  SkysphereShader(Shader& renderShader);
  void update(const glm::mat4 &projectionView, const glm::vec3 &lightDir);
  void setSkysphereType(int type, const glm::mat4 &transform);

private:
  Shader& renderShader;
};
