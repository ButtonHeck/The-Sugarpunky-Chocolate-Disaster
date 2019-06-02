/*
 * Copyright 2019 Ilya Malgin
 * SkysphereFacade.h
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
 * Purpose: contains declaration for SkysphereFacade class
 * @version 0.1.0
 */

#ifndef SKYSPHEREFACADE_H
#define SKYSPHEREFACADE_H
#include "Skysphere.h"
#include "SkysphereShader.h"

/**
 * @brief Represents a facade to skysphere related code module.
 * Contains two different skyspheres for each purpose and a personal shader manager.
 * Responsible for delegating tasks between its member objects during rendering of skyspheres
 */
class SkysphereFacade
{
public:
  SkysphereFacade(Shader& renderShader);
  void draw(const glm::mat4 &transform,
            const glm::mat4 &projectionView,
            const glm::vec3 &lightDir);
  void moveStarsSkysphere(float angleDegrees);

private:
  //define possible types of spheres
  enum SKYSPHERE_TYPE : int
  {
    SKYSPHERE_AMBIENT_LIGHTING = 0,
    SKYSPHERE_STARS = 1
  };
  const glm::vec3 SKYSPHERE_STARS_ROTATION = glm::normalize(glm::vec3(-0.5f, 1.0f, -0.2f));

  Skysphere theSunAmbientLightingHemisphere, starsSkysphere;
  SkysphereShader shader;
};

#endif // SKYSPHEREFACADE_H
