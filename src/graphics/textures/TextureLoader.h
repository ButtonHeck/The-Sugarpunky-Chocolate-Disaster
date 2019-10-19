/*
 * Copyright 2019 Ilya Malgin
 * TextureLoader.h
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
 * Purpose: contains declaration for TextureLoader class
 * @version 0.1.0
 */

#pragma once

#include "TypeAliases"

#include <GL/glew.h>

class ScreenResolution;

/**
* @brief utility class for loading/creating textures, setting textures parameters and stuff
*/
class TextureLoader
{
public:
	TextureLoader( const ScreenResolution & screenResolution ) noexcept;
	GLuint loadTexture( const std::string & path,
						GLuint textureUnit,
						GLenum wrapType,
						GLint magFilter,
						GLint minFilter,
						bool useAnisotropy,
						bool isBindless = false,
						bool explicitNoSRGB = false );
	GLuint createFrameMSTexture( GLuint textureUnit, 
								 int multisamples ) noexcept;
	GLuint createFrameTexture( GLuint textureUnit, 
							   bool isDepthTexture ) noexcept;
	GLuint createFrameTextureSized( GLuint textureUnit, 
									bool isDepthTexture, 
									int width, 
									int height, 
									bool useAnisotropy ) noexcept;
	GLuint createDepthMapTexture( GLuint textureUnit, 
								  int width, 
								  int height ) noexcept;
	GLuint loadCubemapResource( const std::string & directory,
								GLuint textureUnit,
								bool explicitNoSRGB );
	GLuint createUnderwaterReliefTexture( GLuint textureUnit, 
										  const map2D_f & waterMap, 
										  GLint magFilter, 
										  GLint minFilter );

private:
	GLuint createTextureObject( GLenum target, 
								GLuint textureUnit, 
								bool isBindless ) noexcept;
	void setTexture2DParameters( GLuint texture, 
								 GLint magFilter, 
								 GLint minFilter, 
								 GLenum wrapType ) noexcept;
	void setTex2DArrayParameters( GLint magFilter, 
								  GLint minFilter, 
								  GLenum wrapType ) noexcept;

	const ScreenResolution & screenResolution;
};
