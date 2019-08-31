/*
 * Copyright 2019 Ilya Malgin
 * Framebuffer.cpp
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
 * Purpose: contains definition for Framebuffer class
 * @version 0.1.0
 */

#include "Framebuffer"
#include "TextureManager"
#include "Logger"

/**
* @brief plain ctor. Creates a framebuffer object on the OpenGL side
*/
Framebuffer::Framebuffer( TextureManager & textureManager ) noexcept
	: textureManager( textureManager )
{
	glCreateFramebuffers( 1, &fbo );
}

/**
* @brief sends delete fbo command to OpenGL
*/
Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers( 1, &fbo );
}

/**
* @brief validates framebuffer object.
* @note Any bindings and initialization is done in descendent framebuffer classes
*/
void Framebuffer::checkStatus()
{
	if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
	{
		Logger::log( "Framebuffer is not complete\n" );
	}
}

/**
* @brief binds framebuffer object to current framebuffer and updates viewport dimensions
* @param viewportWidth width of a viewport used with this fbo
* @param viewportHeight height of a viewport used with this fbo
*/
void Framebuffer::bindToViewport( int viewportWidth, 
								  int viewportHeight ) noexcept
{
	glBindFramebuffer( GL_FRAMEBUFFER, fbo );
	glViewport( 0, 0, viewportWidth, viewportHeight );
}

/**
* @brief binds 0 to current framebuffer (thus unbinding fbo) and updates viewport dimensions
* @param viewportWidth width of a viewport used after this fbo has been used
* @param viewportHeight height of a viewport used after this fbo has been used
*/
void Framebuffer::unbindToViewport( int viewportWidth, 
									int viewportHeight ) noexcept
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glViewport( 0, 0, viewportWidth, viewportHeight );
}
