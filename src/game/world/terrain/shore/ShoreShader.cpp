/*
 * Copyright 2019 Ilya Malgin
 * ShoreShader.cpp
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
 * Purpose: contains definitions for ShoreShader class
 * @version 0.1.0
 */

#include "ShoreShader"
#include "Shader"

/**
* @brief plain ctor
* @param renderShader shader program used for onscreen rendering
* @param normalsShader shader program used for onscreen rendering of normals
*/
ShoreShader::ShoreShader(Shader &renderShader, Shader &normalsShader) noexcept
  :
    renderShader(renderShader),
    normalsShader(normalsShader)
{}

/**
* @brief updates shader program uniforms
* @param lightDir sunlight direction vector
* @param lightSpaceMatrices array of "projection * view" matrices from the Sun point of view
* @param projectionView "projection * view" matrix
* @param useShadows indicator of whether shadows should be calculated
* @param useClipDistanceReflection indicator of whether clip distance will be used by OpenGL for reflection rendering
* @param useClipDistanceRefraction indicator of whether clip distance will be used by OpenGL for refraction rendering
*/
void ShoreShader::update(const glm::vec3 &lightDir,
                         const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                         const glm::mat4 &projectionView,
                         bool useShadows,
                         bool useClipDistanceReflection,
                         bool useClipDistanceRefraction)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setBool("u_shadowEnable", useShadows);
  renderShader.setVec3("u_lightDir", -lightDir);
  renderShader.setMat4("u_lightSpaceMatrix[0]", lightSpaceMatrices[0]);
  renderShader.setMat4("u_lightSpaceMatrix[1]", lightSpaceMatrices[1]);
  renderShader.setMat4("u_lightSpaceMatrix[2]", lightSpaceMatrices[2]);
  renderShader.setBool("u_useClipDistanceReflection", useClipDistanceReflection);
  renderShader.setBool("u_useClipDistanceRefraction", useClipDistanceRefraction);
}

/**
* @brief updates normals shader program uniforms
* @param projectionView "projection * view" matrix
* @todo remove this in release version of the game
*/
void ShoreShader::updateNormals(const glm::mat4 &projectionView)
{
  normalsShader.use();
  normalsShader.setMat4("u_projectionView", projectionView);
}

/**
* @brief sets debug render mode for shader program
* @param enable indicator of whether debug rendering mode is on
* @todo remove this in release version of the game
*/
void ShoreShader::debugRenderMode(bool enable)
{
  renderShader.use();
  renderShader.setBool("u_debugRenderMode", enable);
}
