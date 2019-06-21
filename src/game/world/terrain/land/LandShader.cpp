/*
 * Copyright 2019 Ilya Malgin
 * LandShader.cpp
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
 * Purpose: contains definitions for LandShader class
 * @version 0.1.0
 */

#include "LandShader"
#include "Shader"

/**
* @brief plain ctor
* @param renderShader shader program used during rendering
*/
LandShader::LandShader(Shader &renderShader) noexcept
  :
    renderShader(renderShader)
{}

/**
* @brief updates shader program uniforms
* @param lightDir sunlight direction vector
* @param lightSpaceMatrices array of "projection * view" matrices from the Sun point of view
* @param projectionView "projection * view" matrix
* @param useShadows indicator of whether shadows should be calculated
*/
void LandShader::update(const glm::vec3 &lightDir,
                        const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                        const glm::mat4 &projectionView,
                        bool useShadows)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setBool("u_shadowEnable", useShadows);
  renderShader.setVec3("u_lightDir", -lightDir);
  renderShader.setMat4("u_lightSpaceMatrix[0]", lightSpaceMatrices[0]);
  renderShader.setMat4("u_lightSpaceMatrix[1]", lightSpaceMatrices[1]);
  renderShader.setMat4("u_lightSpaceMatrix[2]", lightSpaceMatrices[2]);
}
