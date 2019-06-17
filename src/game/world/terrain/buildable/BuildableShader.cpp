/*
 * Copyright 2019 Ilya Malgin
 * BuildableShader.cpp
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
 * Purpose: contains definition for BuildableShader class
 * @version 0.1.0
 */

#include "BuildableShader"
#include "Shader"

/**
* @brief plain ctor
* @param buildableRenderShader shader used during buildable tiles rendering
* @param selectedRenderShader shader used during selected tile rendering
*/
BuildableShader::BuildableShader(Shader &buildableRenderShader, Shader &selectedRenderShader) noexcept
  :
    buildableRenderShader(buildableRenderShader),
    selectedRenderShader(selectedRenderShader)
{}

/**
* @brief updates necessary uniforms in buildable shader
* @param projectionView "projection * view" matrix
*/
void BuildableShader::updateBuildable(const glm::mat4 &projectionView)
{
  buildableRenderShader.use();
  buildableRenderShader.setMat4("u_projectionView", projectionView);
}

/**
* @brief updates necessary uniforms in selected shader
* @param projectionView "projection * view" matrix
* @param selectedTranslation translation vector of the selected cell geometry
*/
void BuildableShader::updateSelected(const glm::mat4 &projectionView, const glm::vec4 &selectedTranslation)
{
  selectedRenderShader.use();
  selectedRenderShader.setMat4("u_projectionView", projectionView);
  selectedRenderShader.setVec4("u_translation", selectedTranslation);
}
