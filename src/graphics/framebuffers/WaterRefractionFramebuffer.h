/*
 * Copyright 2019 Ilya Malgin
 * WaterRefractionFramebuffer.h
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
 * Purpose: contains declaration for WaterRefractionFramebuffer class
 * @version 0.1.0
 */

#pragma once

#include "Framebuffer"

/**
* @brief framebuffer for world refraction image rendering from water point of view
* @note for depth component this framebuffer uses texture which is used in the water shader
*/
class WaterRefractionFramebuffer : public Framebuffer
{
public:
  WaterRefractionFramebuffer(TextureManager& textureManager) noexcept;
  virtual void setup() override;
};
