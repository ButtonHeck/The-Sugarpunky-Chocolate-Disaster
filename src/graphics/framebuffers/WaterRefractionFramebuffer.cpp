/*
 * Copyright 2019 Ilya Malgin
 * WaterRefractionFramebuffer.cpp
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
 * Purpose: contains definitions for WaterRefractionFramebuffer class
 * @version 0.1.0
 */

#include "WaterRefractionFramebuffer"
#include "TextureManager"

/**
* @brief plain ctor
* @param textureManager texture manager
*/
WaterRefractionFramebuffer::WaterRefractionFramebuffer(TextureManager &textureManager) noexcept
  :
    Framebuffer(textureManager)
{}

/**
* @brief initializes framebuffer to valid state
*/
void WaterRefractionFramebuffer::setup()
{
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager.get(TEX_FRAME_WATER_REFRACTION), 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureManager.get(TEX_FRAME_WATER_REFRACTION_DEPTH), 0);
  checkStatus();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
