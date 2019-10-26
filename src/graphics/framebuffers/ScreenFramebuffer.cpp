/*
 * Copyright 2019 Ilya Malgin
 * ScreenFramebuffer.cpp
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
 * Purpose: contains definitions for ScreenFramebuffer class
 * @version 0.1.0
 */

#include "ScreenFramebuffer"
#include "TextureManager"
#include "BenchmarkTimer"
#include "ShaderManager"
#include "Shader"
#include "ScreenResolution"
#include "SettingsManager"

/**
* @brief plain ctor, externally creates multisample framebuffer object and related renderbuffer
* @param textureManager texture manager
* @param screenResolution current resolution of the screen
* @param shaderManager global shader programs manager
*/
ScreenFramebuffer::ScreenFramebuffer( TextureManager & textureManager, 
									  const ScreenResolution & screenResolution, 
									  ShaderManager & shaderManager )
	: Framebuffer( textureManager )
	, screenResolution( screenResolution )
	, shaderManager( shaderManager )
	, screenBuffers( VAO | VBO )
{
	glCreateFramebuffers( 1, &multisampleFbo );
	glCreateRenderbuffers( 1, &multisampleDepthRbo );
}

/**
* @brief externally sends delete command to OpenGL for multisample FBO/Renderbuffer objects
*/
ScreenFramebuffer::~ScreenFramebuffer()
{
	glDeleteFramebuffers( 1, &multisampleFbo );
	glDeleteRenderbuffers( 1, &multisampleDepthRbo );
}

/**
* @brief prepares framebuffers to valid state and initializes buffer collections
*/
void ScreenFramebuffer::setup()
{
	BENCHMARK( "ScreenBuffer: setup", false );
	setupFramebuffers();
	setupScreenQuadBuffer();
}

/**
* @brief prepares framebuffer to valid state
*/
void ScreenFramebuffer::setupFramebuffers()
{
	//multisample
	glBindFramebuffer( GL_FRAMEBUFFER, multisampleFbo );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureManager.get( TEX_FRAME_MULTISAMPLED ), 0 );
	glBindRenderbuffer( GL_RENDERBUFFER, multisampleDepthRbo );
	glRenderbufferStorageMultisample( GL_RENDERBUFFER, SettingsManager::getInt("GRAPHICS", "multisamples"), GL_DEPTH_COMPONENT24, screenResolution.getWidth(), screenResolution.getHeight() );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, multisampleDepthRbo );
	checkStatus();
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	//intermediate FBO (or direct off-screen FBO without multisampling)
	glBindFramebuffer( GL_FRAMEBUFFER, fbo );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
							SettingsManager::getBool( "GRAPHICS", "hdr" ) ? textureManager.get( TEX_FRAME_HDR ) : textureManager.get( TEX_FRAME ), 0 );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureManager.get( TEX_FRAME_DEPTH ), 0 );
	checkStatus();
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

/**
* @brief initializes screen quad buffer that holds rendered image as a frame texture
*/
void ScreenFramebuffer::setupScreenQuadBuffer()
{
	screenBuffers.bind( VAO | VBO );
	constexpr float SCREEN_VERTICES[] = {
	  -1.0f, -1.0f, 0.0f, 0.0f,
	   1.0f, -1.0f, 1.0f, 0.0f,
	   1.0f,  1.0f, 1.0f, 1.0f,
	   1.0f,  1.0f, 1.0f, 1.0f,
	  -1.0f,  1.0f, 0.0f, 1.0f,
	  -1.0f, -1.0f, 0.0f, 0.0f
	};
	glBufferData( GL_ARRAY_BUFFER, sizeof( SCREEN_VERTICES ), &SCREEN_VERTICES, GL_STATIC_DRAW );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), (void*)0 );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), (void*)( 2 * sizeof( float ) ) );
	BufferCollection::bindZero( VAO | VBO );
}

/**
* @brief sets GL framebuffer targets and delegates draw call to OpenGL to render screen quad
* containing texture of current frame
* @param useMultisampling multisampling mode flag
* @param useDOF depth-of-field mode flag
* @param useVignette vignette filter flag
*/
void ScreenFramebuffer::draw( bool useMultisampling, 
							  bool useDOF,
							  bool useVignette )
{
	BENCHMARK( "ScreenBuffer: draw", true );
	if( useMultisampling )
	{
		glBindFramebuffer( GL_READ_FRAMEBUFFER, multisampleFbo );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fbo );
		glBlitFramebuffer( 0, 0, screenResolution.getWidth(), screenResolution.getHeight(),
						   0, 0, screenResolution.getWidth(), screenResolution.getHeight(),
						   GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST );
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}
	else
	{
		glBindFramebuffer( GL_READ_FRAMEBUFFER, fbo );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
	}

	//activate shader and set DOF uniform state
	Shader & screenShader = shaderManager.get( SHADER_MS_TO_DEFAULT );
	screenShader.use();
	screenShader.setBool( "u_useDOF", useDOF );
	screenShader.setBool( "u_useVignette", useVignette );

	//render frame texture onto screen
	screenBuffers.bind( VAO );
	glDisable( GL_DEPTH_TEST );
	glDrawArrays( GL_TRIANGLES, 0, VERTICES_PER_QUAD );
	glEnable( GL_DEPTH_TEST );
}

/**
* @brief sends bind framebuffer command to OpenGL
* @param enableMultisampling flag defininng which framebuffer object should be bound
*/
void ScreenFramebuffer::bindAppropriateFBO( bool enableMultisampling ) noexcept
{
	glBindFramebuffer( GL_FRAMEBUFFER, enableMultisampling ? multisampleFbo : fbo );
}
