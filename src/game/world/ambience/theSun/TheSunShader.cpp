/*
 * Copyright 2019 Ilya Malgin
 * TheSunShader.cpp
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
 * Purpose: contains definitions for TheSunShader class
 * @version 0.1.0
 */

#include "TheSunShader"
#include "Shader"

/**
 * @param renderShader compiled shader program to use during rendering
 */
TheSunShader::TheSunShader(Shader &renderShader) noexcept
  :
    renderShader(renderShader)
{}

/**
 * @brief activates shader program and updates its uniforms
 * @param projectionView Projection*View matrix
 * @param model Model matrix
 */
void TheSunShader::update(const glm::mat4 &projectionView, const glm::mat4& model)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setMat4("u_model", model);
}
