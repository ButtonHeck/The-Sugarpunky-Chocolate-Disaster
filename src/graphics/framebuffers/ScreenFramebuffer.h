/*
 * Copyright 2019 Ilya Malgin
 * ScreenFramebuffer.h
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
 * Purpose: contains declaration for ScreenFramebuffer class
 * @version 0.1.0
 */

#pragma once

#include "Framebuffer"
#include "BufferCollection"

class ShaderManager;
class ScreenResolution;

/**
* @brief framebuffer for plain onscreen rendering. Technically there are two GL framebuffers, one of which
* is used for multisample rendering
*/
class ScreenFramebuffer : public Framebuffer
{
public:
	ScreenFramebuffer( TextureManager & textureManager, 
					   const ScreenResolution & screenResolution, 
					   ShaderManager & shaderManager );
	virtual ~ScreenFramebuffer();
	virtual void setup() override;
	void bindToViewport( int viewportWidth, 
						 int viewportHeight ) = delete;
	void unbindToViewport( int viewportWidth, 
						   int viewportHeight ) = delete;
	void draw( bool useMultisampling, 
			   bool useDOF );
	void bindAppropriateFBO( bool enableMultisampling ) noexcept;

private:
	void setupFramebuffers();
	void setupScreenQuadBuffer();

	const ScreenResolution & screenResolution;
	ShaderManager & shaderManager;
	BufferCollection screenBuffers;
	GLuint multisampleDepthRbo;
	GLuint multisampleFbo;
};
