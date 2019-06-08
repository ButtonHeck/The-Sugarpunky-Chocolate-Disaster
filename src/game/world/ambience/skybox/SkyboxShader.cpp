/*
 * Copyright 2019 Ilya Malgin
 * SkyboxShader.cpp
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
 * Purpose: contains definitions for SkyboxShader class
 * @version 0.1.0
 */

#include "SkyboxShader"

/**
 * @param renderShader precompiled shader program which is used during rendering
 */
SkyboxShader::SkyboxShader(Shader &renderShader)
  :
    renderShader(renderShader)
{}

/**
 * @brief activates shader program and updates its uniform variables
 * @param projectionView Projection*View matrix
 * @param viewPosition current position of the camera
 * @param lightDir direction of the sunlight (directional lighting)
 */
void SkyboxShader::update(const glm::mat4 &projectionView,
                          const glm::vec3 &viewPosition,
                          const glm::vec3 &lightDir)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setVec3("u_viewPosition", viewPosition);
  //send this as reverse direction
  renderShader.setVec3("u_lightDir", -lightDir);
}

/**
 * @brief sets integer "type" uniform in the shader, matched with shader source code types
 * @param type integer representation of the skybox type
 */
void SkyboxShader::selectSkyboxType(int type)
{
  renderShader.setInt("u_type", type);
}
