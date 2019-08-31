/*
 * Copyright 2019 Ilya Malgin
 * WaterReflectionFramebuffer.h
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
 * Purpose: contains declaration for WaterReflectionFramebuffer class
 * @version 0.1.0
 */

#pragma once

#include "Framebuffer"

/**
* @brief framebuffer for world reflection image rendering from water point of view
* @note for depth component this framebuffer uses additional renderbuffer object just for the sake of framebuffer completion
*/
class WaterReflectionFramebuffer : public Framebuffer
{
public:
	WaterReflectionFramebuffer( TextureManager & textureManager ) noexcept;
	virtual ~WaterReflectionFramebuffer();
	virtual void setup() override;

private:
	GLuint rbo;
};
