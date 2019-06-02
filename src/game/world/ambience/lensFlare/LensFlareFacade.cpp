/*
 * Copyright 2019 Ilya Malgin
 * LensFlareFacade.cpp
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
 * Purpose: contains definitions for LensFlareFacade class
 * @version 0.1.0
 */

#include "LensFlareFacade.h"

/**
 * @param shader compiled program fed to the dedicated shader object
 * @param textureLoader loader for flares
 * @param screenResolution current screen resolution used for flares points size initialization
 */
LensFlareFacade::LensFlareFacade(Shader& shader, TextureLoader &textureLoader, const ScreenResolution &screenResolution)
  :
    BRIGHTNESS_HALO(1.25f),
    BRIGHTNESS_FLARES(0.55f),
    FLARES_SPACING(0.38f),
    basicGLBuffers(VAO | VBO),
    shader(shader),
    renderer(basicGLBuffers)
{
  flares.reserve(NUM_LENS_FLARES);
  flares.emplace_back(1300.0f, textureLoader.loadTexture("lensFlares/flare11.png", 0, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false, true, true));
  flares.emplace_back(220.0f, textureLoader.loadTexture("lensFlares/flare2.png", 0, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false, true, true));
  flares.emplace_back(150.0f, textureLoader.loadTexture("lensFlares/flare5.png", 0, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false, true, true));
  flares.emplace_back(75.0f, textureLoader.loadTexture("lensFlares/flare6.png", 0, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false, true, true));
  flares.emplace_back(160.0f, textureLoader.loadTexture("lensFlares/flare1.png", 0, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false, true, true));
  flares.emplace_back(180.0f, textureLoader.loadTexture("lensFlares/flare9.png", 0, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false, true, true));
  flares.emplace_back(340.0f, textureLoader.loadTexture("lensFlares/flare3.png", 0, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false, true, true));

  /*
   * register flare textures in the dedicated bindless texture manager's vector,
   * a flare "knows" itself what texture to use (see related shader file)
   */
  for (unsigned int i = 0; i < NUM_LENS_FLARES; i++)
    BindlessTextureManager::emplaceBack("u_flares[" + std::to_string(i) + "]", flares[i].getTextureID(), BINDLESS_TEXTURE_LENS_FLARE);

  basicGLBuffers.bind(VAO | VBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

  //calculate approximated correct size of flares points based on a current screen resolution
  float pointSizeDivisorX = screenResolution.getWidthRatioToReference();
  float pointSizeDivisorY = screenResolution.getHeightRatioToReference();
  float pointSizeDivisor = (pointSizeDivisorX + pointSizeDivisorY) / 2;
  for (LensFlareElement& flare : flares)
    flare.adjustPointSize(pointSizeDivisor);
}

/**
 * @brief prepare both shader and renderer to rendering flares if the sun is close enough to the screen center
 * @param sunWorldPosition position of the Sun in the world space
 * @param projectionView precalculated Projection*View matrix
 * @param theSunVisibility defines how much the Sun is visible
 */
void LensFlareFacade::draw(const glm::vec3& sunWorldPosition, const glm::mat4& projectionView, float theSunVisibility)
{
  glm::vec4 sunCameraSpacePosition = projectionView * glm::vec4(sunWorldPosition, 1.0f);
  if (sunCameraSpacePosition.w > 0) //otherwise we obviously could not see any flares
    {
      //map x,y from NDC[-1;1] to the texture coordinates[0;1] with y-axis reflected
      float x = (sunCameraSpacePosition.x / sunCameraSpacePosition.w + 1) / 2.0f;
      float y = 1.0f - ((sunCameraSpacePosition.y / sunCameraSpacePosition.w + 1) / 2.0f);
      glm::vec2 sunScreenPosition(x, y);
      glm::vec2 sunScreenPositionToScreenCenter = glm::vec2(0.5f) - sunScreenPosition;
      float sunToCenterLength = glm::length(sunScreenPositionToScreenCenter);
      float brightnessFlares = glm::max((1 - sunToCenterLength / BRIGHTNESS_FLARES) * theSunVisibility, 0.0f);
      float brightnessHalo = glm::max((1 - sunToCenterLength / BRIGHTNESS_HALO) * theSunVisibility, 0.0f);
      updateFlaresPositions(sunScreenPosition, sunScreenPositionToScreenCenter);
      shader.update(brightnessFlares, brightnessHalo);
      renderer.draw(NUM_LENS_FLARES);
    }
}

/**
 * @brief updates vertex buffer data depending on input values and sends it to the GPU
 * @param sunScreenPosition defines the starting point for locating flares
 * @param sunScreenPositionToScreenCenter defines a direction of a line along which flares should be positioned on screen
 */
void LensFlareFacade::updateFlaresPositions(const glm::vec2 &sunScreenPosition, const glm::vec2 &sunScreenPositionToScreenCenter)
{
  for (unsigned int flareIndex = 0; flareIndex < flares.size(); flareIndex++)
    {
      flares[flareIndex].setPosition(sunScreenPosition + flareIndex * FLARES_SPACING * sunScreenPositionToScreenCenter);
      vertices[flareIndex*3 + 0] = flares[flareIndex].getPosition().x;
      vertices[flareIndex*3 + 1] = flares[flareIndex].getPosition().y;
      vertices[flareIndex*3 + 2] = flares[flareIndex].getPointSize();
    }
  basicGLBuffers.bind(VAO | VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}
