/*
 * Copyright 2019 Ilya Malgin
 * SkyboxFacade.h
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
 * Purpose: contains declaration for SkyboxFacade class
 * @version 0.1.0
 */

#ifndef SKYBOXFACADE_H
#define SKYBOXFACADE_H
#include "game/world/ambience/skybox/Skybox.h"
#include "game/world/ambience/skybox/SkyboxShader.h"
#include "util/BenchmarkTimer.h"

/**
 * @brief Represents a facade for skybox in the game, contains a single skybox object and personal shader.
 * Responsible for delegating tasks between its member objects during rendering of skyboxes
 */
class SkyboxFacade
{
public:
  SkyboxFacade(Shader& renderShader);
  void draw(const glm::mat4 &skyboxProjectionView,
            const glm::vec3 &viewPosition,
            const glm::vec3 &lightDir);

private:
  //define three possible modes of skybox for shader
  enum SKYBOX_TYPE : int
  {
    SKYBOX_HILLS_NEAR = 0,
    SKYBOX_HIILS_FAR = 1,
    SKYBOX_CLOUDS = 2
  };
  SkyboxShader shader;
  Skybox skybox;
};

#endif // SKYBOXFACADE_H
