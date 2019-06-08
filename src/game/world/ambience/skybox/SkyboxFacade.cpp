/*
 * Copyright 2019 Ilya Malgin
 * SkyboxFacade.cpp
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
 * Purpose: contains definitions for SkyboxFacade class
 * @version 0.1.0
 */

#include "SkyboxFacade"

/**
 * @param renderShader compiled program fed to personal shader manager
 */
SkyboxFacade::SkyboxFacade(Shader &renderShader)
  :
    shader(renderShader),
    skybox()
{}

/**
 * @brief manages shader program object state changes during rendering and delegates draw call
 * invocations to skybox member object
 * @param skyboxProjectionView Projection*View matrix
 * @param viewPosition current position of the camera
 * @param lightDir direction of the sunlight (directional lighting)
 */
void SkyboxFacade::draw(const glm::mat4 &skyboxProjectionView,
                        const glm::vec3 &viewPosition,
                        const glm::vec3 &lightDir)
{
  BENCHMARK("SkyboxFacade: draw all boxes", true);
  shader.update(skyboxProjectionView, viewPosition, lightDir);
  shader.selectSkyboxType(SKYBOX_CLOUDS);
  skybox.draw();
  shader.selectSkyboxType(SKYBOX_HILLS_NEAR);
  skybox.draw();
  shader.selectSkyboxType(SKYBOX_HIILS_FAR);
  skybox.draw();
}
