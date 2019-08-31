/*
 * Copyright 2019 Ilya Malgin
 * DepthmapFramebuffer.cpp
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
 * Purpose: contains definition for DepthmapFramebuffer class
 * @version 0.1.0
 */

#include "DepthmapFramebuffer"
#include "TextureManager"

/**
* @brief plain ctor
* @param textureManager texture manager to fetch depthmap texture unit from later on
*/
DepthmapFramebuffer::DepthmapFramebuffer( TextureManager & textureManager ) noexcept
	: Framebuffer( textureManager )
{}

/**
* @brief initializes framebuffer
*/
void DepthmapFramebuffer::setup()
{
	glBindFramebuffer( GL_FRAMEBUFFER, fbo );
	glNamedFramebufferTexture( fbo, GL_DEPTH_ATTACHMENT, textureManager.get( TEX_DEPTH_MAP_SUN ), 0 );
	glDrawBuffer( GL_NONE );
	glReadBuffer( GL_NONE );
	checkStatus();
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}
