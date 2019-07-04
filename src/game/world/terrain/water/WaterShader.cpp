/*
 * Copyright 2019 Ilya Malgin
 * WaterShader.cpp
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
 * Purpose: contains definitions for WaterShader class
 * @version 0.1.0
 */

#include "WaterShader"
#include "Frustum"
#include "Shader"

#include <GLFW/glfw3.h>

/**
* @brief plain ctor
* @param renderShader shader program used for plain onscreen rendering
* @param cullingShader shader program used for offscreen rendering with frustum culling
* @param normalsShader shader program used for onscreen rendering of water normals
*/
WaterShader::WaterShader(Shader &renderShader, Shader &cullingShader, Shader &normalsShader) noexcept
  :
    renderShader(renderShader),
    cullingShader(cullingShader),
    normalsShader(normalsShader)
{}

/**
* @brief update frustum culling shader program according to transform feedback usage
* @note it is programmer's responsibility to keep client data matched with shader program itself
*/
void WaterShader::setupCulling()
{
  const unsigned int TRANSFORM_FEEDBACK_OUTPUT_ATTRIBUTES_COUNT = 1;
  const GLchar* varyings[TRANSFORM_FEEDBACK_OUTPUT_ATTRIBUTES_COUNT] = {"o_pos"};
  glTransformFeedbackVaryings(cullingShader.getID(), TRANSFORM_FEEDBACK_OUTPUT_ATTRIBUTES_COUNT, varyings, GL_INTERLEAVED_ATTRIBS);
  cullingShader.link();
}

/**
* @brief updates shader program uniforms
* @param lightDir sunlight direction vector
* @param lightSpaceMatrices array of "projection * view" matrices from the Sun point of view
* @param projectionView "projection * view" matrix
* @param viewPosition position vector of the camera
* @param viewFrustum view frustum of the camera
* @param useFrustumCulling indicator of whether to use frustum culling when drawing
*/
void WaterShader::update(const glm::vec3 &lightDir,
                         const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                         const glm::mat4 &projectionView,
                         const glm::vec3 &viewPosition,
                         const Frustum &viewFrustum,
                         bool useFrustumCulling)
{
  //declared static because this variable should keep its value from call to call
  static float dudvMoveOffset = 0.0f;
  if (useFrustumCulling)
    {
      cullingShader.use();
      cullingShader.setVec4("u_frustumPlanes[0]", viewFrustum.getPlane(FRUSTUM_LEFT));
      cullingShader.setVec4("u_frustumPlanes[1]", viewFrustum.getPlane(FRUSTUM_RIGHT));
      cullingShader.setVec4("u_frustumPlanes[2]", viewFrustum.getPlane(FRUSTUM_BOTTOM));
      cullingShader.setVec4("u_frustumPlanes[3]", viewFrustum.getPlane(FRUSTUM_TOP));
      cullingShader.setVec4("u_frustumPlanes[4]", viewFrustum.getPlane(FRUSTUM_BACK));
    }
  renderShader.use();
  renderShader.setFloat("u_time", glfwGetTime());
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setVec3("u_viewPosition", viewPosition);
  renderShader.setVec3("u_lightDir", -lightDir);
  renderShader.setMat4("u_lightSpaceMatrix[0]", lightSpaceMatrices[0]);
  renderShader.setMat4("u_lightSpaceMatrix[1]", lightSpaceMatrices[1]);
  renderShader.setMat4("u_lightSpaceMatrix[2]", lightSpaceMatrices[2]);
  renderShader.setFloat("u_dudvMoveOffset", dudvMoveOffset);

  const float DUDV_ANIMATION_SPEED = 0.0004f;
  dudvMoveOffset += DUDV_ANIMATION_SPEED;
  if (dudvMoveOffset >= 2.0f)
    dudvMoveOffset = 0.0f;
}

/**
* @brief updates uniforms of the shader program for normals rendering
* @param projectionView "projection * view" matrix
* @todo remove this in release version of the game 
*/
void WaterShader::updateNormals(const glm::mat4 &projectionView)
{
  normalsShader.use();
  normalsShader.setMat4("u_projectionView", projectionView);
}

/**
* @brief switches debug mode for render shader
* @param enable on/off flag for visual debugging mode
* @todo remove from release version of the game
*/
void WaterShader::debugRenderMode(bool enable)
{
  renderShader.use();
  renderShader.setBool("u_debugRenderMode", enable);
}
