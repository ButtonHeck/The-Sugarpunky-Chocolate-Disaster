/*
 * Copyright 2019 Ilya Malgin
 * SkyboxShader.h
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
 * Purpose: contains declaration for SkyboxShader class
 * @version 0.1.0
 */

#ifndef SKYBOXSHADER_H
#define SKYBOXSHADER_H
#include "../../src/graphics/shaders/Shader.h"

/**
 * @brief Personal shader manager for skybox related code module, responsible for updating non-constant uniform variables
 * and switching between multiple skybox types
 */
class SkyboxShader
{
public:
  SkyboxShader(Shader& renderShader);
  void update(const glm::mat4 &projectionView,
              const glm::vec3 &viewPosition,
              const glm::vec3 &lightDir);
  void selectSkyboxType(int type);

private:
  Shader& renderShader;
};

#endif // SKYBOXSHADER_H
