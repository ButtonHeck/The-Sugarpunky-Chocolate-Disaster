/*
 * Copyright 2019 Ilya Malgin
 * PlantsShader.cpp
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
 * Purpose: contains definitions for PlantsShader class
 * @version 0.1.0
 */

#include "PlantsShader"
#include "Shader"

#include <GLFW/glfw3.h>

/**
 * @param renderPhongShader compiled Phong shader program to use during rendering
 * @param renderGouraudShader compiled Gouraud shader program to use during rendering
 */
PlantsShader::PlantsShader(Shader &renderPhongShader, Shader &renderGouraudShader)
  :
    renderPhongShader(renderPhongShader),
    renderGouraudShader(renderGouraudShader)
{}

/**
 * @brief activates necessary shader program and updates its uniforms
 * @param usePhongShading define what shading model to use
 * @param lightDir direction of the sunlight (directional lighting)
 * @param lightSpaceMatrices matrices for shadow sampling
 * @param projectionView Projection*View matrix
 * @param viewPosition current position of the camera
 * @param useShadows define whether to use shadows
 * @param useLandBlending define whether to use blending
 */
void PlantsShader::updateAllPlants(bool usePhongShading,
                                   const glm::vec3 &lightDir,
                                   const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                                   const glm::mat4 &projectionView,
                                   const glm::vec3 &viewPosition,
                                   bool useShadows,
                                   bool useLandBlending)
{
  currentShader = usePhongShading ? &renderPhongShader : &renderGouraudShader;
  currentShader->use();
  currentShader->setMat4("u_projectionView", projectionView);
  currentShader->setVec3("u_viewPosition", viewPosition);
  currentShader->setBool("u_shadowEnable", useShadows);
  currentShader->setBool("u_useLandBlending", useLandBlending);
  //send this as reverse direction
  currentShader->setVec3("u_lightDir", -lightDir);
  currentShader->setMat4("u_lightSpaceMatrix[0]", lightSpaceMatrices[0]);
  currentShader->setMat4("u_lightSpaceMatrix[1]", lightSpaceMatrices[1]);
  currentShader->setMat4("u_lightSpaceMatrix[2]", lightSpaceMatrices[2]);
}

/**
 * @brief update grass position distribution for this frame
 */
void PlantsShader::updateGrassKeyframe()
{
  currentShader->setFloat("u_grassPosDistrubution", glfwGetTime() * 4.3f);
}

/**
 * @brief sets integer "type" of current plant to draw, matches with shader source code types
 * @param type integer representation of plant type
 */
void PlantsShader::setType(int type)
{
  currentShader->setInt("u_type", type);
}

/**
 * @brief tells shader whether plain or low-poly model is currently being rendered
 * @param isLowPoly bool flag to shader program
 */
void PlantsShader::setLowPolyMode(bool isLowPoly)
{
  currentShader->setBool("u_isLowPoly", isLowPoly);
}
