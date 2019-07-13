/*
 * Copyright 2019 Ilya Malgin
 * UnderwaterShader.cpp
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
 * Purpose: contains definitions for UnderwaterShader class
 * @version 0.1.0
 */

#include "UnderwaterShader"
#include "Shader"

/**
* @brief plain ctor
* @param renderShader shader program used for rendering
*/
UnderwaterShader::UnderwaterShader(Shader &renderShader) noexcept
  :
    renderShader(renderShader)
{}

/**
* @brief updates shader program uniforms
* @param lightDir sunlight direction vector
* @param projectionView "projection * view" matrix
* @param useDesaturation define whether to apply desaturation
*/
void UnderwaterShader::update(const glm::vec3 &lightDir, const glm::mat4 &projectionView, bool useDesaturation)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setVec3("u_lightDir", -lightDir);
  renderShader.setBool("u_useDesaturation", useDesaturation);
}
