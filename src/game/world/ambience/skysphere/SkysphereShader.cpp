/*
 * Copyright 2019 Ilya Malgin
 * SkysphereShader.cpp
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
 * Purpose: contains definitions for SkysphereShader class
 * @version 0.1.0
 */

#include "SkysphereShader.h"

/**
 * @brief SkysphereShader::SkysphereShader
 * @param renderShader precompiled shader program which is used during rendering
 */
SkysphereShader::SkysphereShader(Shader &renderShader)
  :
    renderShader(renderShader)
{}

/**
 * @brief activates shader program and updates its uniforms
 * @param projectionView Projection*View matrix
 * @param lightDir direction of the sunlight (directional lighting)
 */
void SkysphereShader::update(const glm::mat4 &projectionView, const glm::vec3 &lightDir)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  //send this as reverse direction
  renderShader.setVec3("u_lightDir", -lightDir);
}

/**
 * @brief sets integer "type" of current skysphere to draw, matched with shader source code types
 * @param type integer representation of skysphere type
 * @param transform matrix to apply for (hemi)sphere
 */
void SkysphereShader::setSkysphereType(int type, const glm::mat4 &transform)
{
  renderShader.setBool("u_type", type);
  renderShader.setMat4("u_model", transform);
}
