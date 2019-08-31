/*
 * Copyright 2019 Ilya Malgin
 * WaterReflectionFramebuffer.cpp
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
 * Purpose: contains definitions for WaterReflectionFramebuffer class
 * @version 0.1.0
 */

#include "WaterReflectionFramebuffer"
#include "GraphicsSettings"
#include "TextureManager"

/**
* @brief plain ctor, additionally sends create renderbuffer call to OpenGL
*/
WaterReflectionFramebuffer::WaterReflectionFramebuffer( TextureManager & textureManager ) noexcept
	: Framebuffer( textureManager )
{
	glCreateRenderbuffers( 1, &rbo );
}

/**
* @brief sends delete renderbuffer call to OpenGL
*/
WaterReflectionFramebuffer::~WaterReflectionFramebuffer()
{
	glDeleteRenderbuffers( 1, &rbo );
}

/**
* @brief initializes framebuffer to valid state
*/
void WaterReflectionFramebuffer::setup()
{
	glBindFramebuffer( GL_FRAMEBUFFER, fbo );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager.get( TEX_FRAME_WATER_REFLECTION ), 0 );
	glBindRenderbuffer( GL_RENDERBUFFER, rbo );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, FRAME_WATER_REFLECTION_WIDTH, FRAME_WATER_REFLECTION_HEIGHT );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo );
	checkStatus();
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}
