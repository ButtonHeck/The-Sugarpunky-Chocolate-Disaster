/*
 * Copyright 2019 Ilya Malgin
 * TextureLoader.cpp
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
 * Purpose: contains definitions for TextureLoader class
 * @version 0.1.0
 */

#include "TextureLoader"
#include "ScreenResolution"
#include "DirectoriesSettings"
#include "GraphicsSettings"
#include "SceneSettings"
#include "Logger"

#include <algorithm>
#include <IL/il.h>

/**
* @brief plain ctor, initializes DevIL library
* @param screenResolution current resolution of the screen
*/
TextureLoader::TextureLoader( const ScreenResolution & screenResolution ) noexcept
	: screenResolution( screenResolution )
{
	ilInit();
	ilEnable( IL_ORIGIN_SET );
	ilOriginFunc( IL_ORIGIN_LOWER_LEFT );
}

/**
* @brief sends create texture command to OpenGL, optionally activates and binds it to appropriate texture slot
* and returns GL defined handler for created texture
* @param target GL defined target to bind a texture
* @param textureUnit texture unit to bind
* @param isBindless defines whether this texture is a bindless one
*/
GLuint TextureLoader::createTextureObject( GLenum target, 
										   GLuint textureUnit, 
										   bool isBindless ) noexcept
{
	GLuint textureID;
	glCreateTextures( target, 1, &textureID );
	if( !isBindless )
	{
		glActiveTexture( GL_TEXTURE0 + textureUnit );
		glBindTexture( target, textureID );
	}
	return textureID;
}

/**
* @brief loads texture from file and initializes its parameters
* @param filename file name of the texture
* @param textureUnit texture unit to bind
* @param wrapType GL defined wrapping mode
* @param magFilter GL defined magnification filter
* @param minFilter GL defined minification filter
* @param useAnisotropy defines whether anisotropic filtering should be applied for this texture
* @param includeCWD defines whether CWD variable should be added to given file name
* @param isBindless defines whether this texture is a bindless one
* @param explicitNoSRGB if true - forces RGB(A) format for texture even if HDR is enabled
*/
GLuint TextureLoader::loadTexture( const std::string & filename,
								   GLuint textureUnit,
								   GLenum wrapType,
								   GLint magFilter,
								   GLint minFilter,
								   bool useAnisotropy,
								   bool includeCWD,
								   bool isBindless,
								   bool explicitNoSRGB )
{
	GLuint textureID = createTextureObject( GL_TEXTURE_2D, textureUnit, isBindless );
	std::string fullname = includeCWD ? std::string( TEXTURES_DIR + filename ) : filename;
	if( !ilLoadImage( fullname.c_str() ) )
	{
		Logger::log( "Error when loading texture: %\n", fullname.c_str() );
	}

	ILubyte * textureData = ilGetData();
	auto textureWidth = ilGetInteger( IL_IMAGE_WIDTH );
	auto textureHeight = ilGetInteger( IL_IMAGE_HEIGHT );
	auto textureChannels = ilGetInteger( IL_IMAGE_CHANNELS );
	GLenum internalFormat;
	GLenum dataFormat;
	if( textureChannels == 4 )
	{
		internalFormat = explicitNoSRGB ? GL_RGBA8 : ( HDR_ENABLED ? GL_SRGB8_ALPHA8 : GL_RGBA8 );
		dataFormat = GL_RGBA;
	}
	else if( textureChannels == 3 )
	{
		internalFormat = explicitNoSRGB ? GL_RGB8 : ( HDR_ENABLED ? GL_SRGB8 : GL_RGB8 );
		dataFormat = GL_RGB;
	}
	else
	{
		throw std::invalid_argument( "Could not handle image with: " + std::to_string( textureChannels ) + " channels" );
	}
	GLsizei mipLevel = ( (GLsizei)log2( std::max( textureWidth, textureHeight ) ) + 1 );

	glTextureStorage2D( textureID, mipLevel, internalFormat, textureWidth, textureHeight );
	glTextureSubImage2D( textureID, 0, 0, 0, textureWidth, textureHeight, dataFormat, GL_UNSIGNED_BYTE, textureData );
	glGenerateTextureMipmap( textureID );
	setTexture2DParameters( textureID, magFilter, minFilter, wrapType );
	if( useAnisotropy )
	{
		glTextureParameterf( textureID, GL_TEXTURE_MAX_ANISOTROPY, ANISOTROPY );
	}

	ilDeleteImage( ilGetInteger( IL_ACTIVE_IMAGE ) );
	return textureID;
}

/**
* @brief creates and initializes frame multisampled texture object
* @param textureUnit texture unit to bind
* @param multisamples multisampling value
*/
GLuint TextureLoader::createFrameMSTexture( GLuint textureUnit, 
											int multisamples ) noexcept
{
	GLuint textureID = createTextureObject( GL_TEXTURE_2D_MULTISAMPLE, textureUnit, false );
	glTextureStorage2DMultisample( textureID, multisamples, GL_RGB16, screenResolution.getWidth(), screenResolution.getHeight(), GL_TRUE );
	return textureID;
}

/**
* @brief creates and initializes frame texture object (no multisampling) with screen resolution dimensions
* @param textureUnit texture unit to bind
* @param isDepthTexture defines whether this texture would be used for depthmap rendering
*/
GLuint TextureLoader::createFrameTexture( GLuint textureUnit, 
										  bool isDepthTexture ) noexcept
{
	return createFrameTextureSized( textureUnit, isDepthTexture, screenResolution.getWidth(), screenResolution.getHeight(), false );
}

/**
* @brief creates and initializes frame texture object (no multisampling) with given dimensions
* @param textureUnit texture unit to bind
* @param isDepthTexture defines whether this texture would be used for depthmap rendering
* @param width width of the texture
* @param height height of the texture
* @param useAnisotropy defines whether anisotropic filtering should be applied for this texture
*/
GLuint TextureLoader::createFrameTextureSized( GLuint textureUnit, 
											   bool isDepthTexture, 
											   int width, 
											   int height, 
											   bool useAnisotropy ) noexcept
{
	GLuint textureID = createTextureObject( GL_TEXTURE_2D, textureUnit, false );
	GLenum format = isDepthTexture ? GL_DEPTH_COMPONENT24 : GL_RGB16;
	glTextureStorage2D( textureID, 1, format, width, height );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	if( useAnisotropy )
	{
		glTextureParameterf( textureID, GL_TEXTURE_MAX_ANISOTROPY, ANISOTROPY );
	}
	return textureID;
}

/**
* @brief creates and initializes depthmap texture (which itself is an array of 2D textures)
* @param textureUnit texture unit to bind
* @param width width of the texture
* @param height height of the texture
*/
GLuint TextureLoader::createDepthMapTexture( GLuint textureUnit, 
											 int width, 
											 int height ) noexcept
{
	GLuint textureID = createTextureObject( GL_TEXTURE_2D_ARRAY, textureUnit, false );
	glTextureStorage3D( textureID, 3, GL_DEPTH_COMPONENT24, width, height, 3 );
	GLfloat borderColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor );
	glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	setTex2DArrayParameters( GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER );
	return textureID;
}

/**
* @brief loads cubemap texture from files of given directory and initializes its parameters
* @param directory directory of the cubemap sides textures
* @param textureUnit texture unit to bind
* @param explicitNoSRGB if true - forces RGB(A) format for texture even if HDR is enabled
* @note it is programmer's responsibility to match filenames of a cubemap sides textures with what this function demands
*/
GLuint TextureLoader::loadCubemap( const std::string & directory, 
								   GLuint textureUnit, 
								   bool explicitNoSRGB )
{
	std::vector<std::string> faces;
	faces.assign(
		{
				TEXTURES_DIR + directory + "right.png",
				TEXTURES_DIR + directory + "left.png",
				TEXTURES_DIR + directory + "up.png",
				TEXTURES_DIR + directory + "down.png",
				TEXTURES_DIR + directory + "front.png",
				TEXTURES_DIR + directory + "back.png"
		} );
	ilOriginFunc( IL_ORIGIN_UPPER_LEFT );

	GLuint textureID = createTextureObject( GL_TEXTURE_CUBE_MAP, textureUnit, false );

	//create and initialize cubemap sides textures
	for( unsigned int i = 0; i < faces.size(); i++ )
	{
		if( !ilLoadImage( faces[i].c_str() ) )
		{
			Logger::log( "Error while loading cubemap element: %\n", faces[i].c_str() );
		}
		auto format = ilGetInteger( IL_IMAGE_FORMAT );
		auto width = ilGetInteger( IL_IMAGE_WIDTH );
		auto height = ilGetInteger( IL_IMAGE_HEIGHT );
		ILubyte * textureData = ilGetData();
		GLenum internalFormat = explicitNoSRGB ? GL_RGBA8 : ( HDR_ENABLED ? GL_SRGB8_ALPHA8 : GL_RGBA8 );
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, textureData );
		ilDeleteImage( ilGetInteger( IL_ACTIVE_IMAGE ) );
	}

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	return textureID;
}

/**
* @brief creates and initialzes underwater texture manually from the water map
* @param textureUnit texture unit to bind
* @param waterMap map of the water
* @param magFilter GL defined magnification filter
* @param minFilter GL defined minification filter
* @note this texture is treated by shader as grayscale color attenuation mask for the actual underwater texture
*/
GLuint TextureLoader::createUnderwaterReliefTexture( GLuint textureUnit, 
													 const map2D_f & waterMap, 
													 GLint magFilter, 
													 GLint minFilter )
{
	static bool needStorage = true;
	static GLuint textureID;
	GLubyte * textureData = new GLubyte[WORLD_WIDTH * WORLD_HEIGHT];
	int left;
	int right;
	int top;
	int bottom;
	float waterCount;

	//walk through water map and fill texture data accordingly
	for( int y = 1; y < WORLD_HEIGHT; y++ )
	{
		for( int x = 0; x < WORLD_WIDTH - 1; x++ )
		{
			left = glm::clamp( x - RIVER_WIDTH_BASE - 3, 0, WORLD_WIDTH - 1 );
			right = glm::clamp( x + RIVER_WIDTH_BASE + 3, 0, WORLD_WIDTH - 1 );
			top = glm::clamp( y - RIVER_WIDTH_BASE - 3, 1, WORLD_HEIGHT );
			bottom = glm::clamp( y + RIVER_WIDTH_BASE + 3, 0, WORLD_HEIGHT );
			waterCount = 0;
			for( int y1 = top; y1 < bottom; y1++ )
			{
				for( int x1 = left; x1 < right; x1++ )
				{
					if( waterMap[y1][x1] != 0 &&
						waterMap[y1 - 1][x1] != 0 &&
						waterMap[y1 - 1][x1 + 1] != 0 &&
						waterMap[y1][x1 + 1] != 0 )
					{
						waterCount += 1.0 - ( ( RIVER_WIDTH_BASE - 4 ) * 0.25 );
					}
				}
			}
			textureData[y * WORLD_WIDTH + x] = (GLubyte)waterCount;
		}
	}

	//only need to allocate this once even if we need to recreate a texture itself
	if( needStorage )
	{
		glCreateTextures( GL_TEXTURE_2D, 1, &textureID );
		glTextureStorage2D( textureID, 1, GL_R8, WORLD_WIDTH, WORLD_HEIGHT );
		needStorage = false;
	}

	glActiveTexture( GL_TEXTURE0 + textureUnit );
	glBindTexture( GL_TEXTURE_2D, textureID );
	glTextureSubImage2D( textureID, 0, 0, 0, WORLD_WIDTH, WORLD_HEIGHT, GL_RED, GL_UNSIGNED_BYTE, textureData );
	setTexture2DParameters( textureID, magFilter, minFilter, GL_REPEAT );
	delete[] textureData;
	return textureID;
}

/**
* @brief helper function that sets commonly used parameters to given texture
* @param texture GL defined texture ID
* @param magFilter GL defined magnification filter
* @param minFilter GL defined minification filter
* @param wrapType GL defined wrapping mode
*/
void TextureLoader::setTexture2DParameters( GLuint texture, 
											GLint magFilter, 
											GLint minFilter, 
											GLenum wrapType ) noexcept
{
	glTextureParameteri( texture, GL_TEXTURE_MAG_FILTER, magFilter );
	glTextureParameteri( texture, GL_TEXTURE_MIN_FILTER, minFilter );
	glTextureParameteri( texture, GL_TEXTURE_WRAP_S, wrapType );
	glTextureParameteri( texture, GL_TEXTURE_WRAP_T, wrapType );
}

/**
* @brief helper functions that sets commonly used parameters to currently bound 2D array texture
* @param magFilter GL defined magnification filter
* @param minFilter GL defined minification filter
* @param wrapType GL defined wrapping mode
*/
void TextureLoader::setTex2DArrayParameters( GLint magFilter, 
											 GLint minFilter, 
											 GLenum wrapType ) noexcept
{
	glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, magFilter );
	glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, minFilter );
	glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrapType );
	glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrapType );
	glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, wrapType );
}
