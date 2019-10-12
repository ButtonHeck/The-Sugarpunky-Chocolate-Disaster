/*
 * Copyright 2019 Ilya Malgin
 * TextureResourceLoader.cpp
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
 * Purpose: contains definitions for TextureResourceLoader class
 * @version 0.1.0
 */

#include "TextureResourceLoader"

#include <string>

/**
* @brief storage for the parsed texture resources
*/
std::unordered_map<std::string, TextureResource> TextureResourceLoader::textures;

/**
* @brief loads data from the .sprd file and creates separate texture resources based on the data
* @param path local name of the texture
* @param flags combination of flags for fopen
*/
void TextureResourceLoader::initialize( const char * path, 
										const char * flags )
{
	errno_t err;
	FILE * file;
	err = fopen_s( &file, path, flags );
	int numTexturesInFile = getc( file );
	textures.reserve( numTexturesInFile );

	for( int textureIndex = 0; textureIndex < numTexturesInFile; textureIndex++ )
	{
		TextureResource textureResource;

		int textureNameLength = getc( file );
		std::unique_ptr<char[]> nameBuffer( new char[textureNameLength + 1] );
		for( size_t charIndex = 0; charIndex < textureNameLength; ++charIndex )
		{
			nameBuffer[charIndex] = getc( file );
		}
		nameBuffer[textureNameLength] = '\0';
		std::string textureName( nameBuffer.get() );
		textureResource.localName = textureName;

		//deserialize texture width
		int texWidthStringLength = getc( file );
		std::unique_ptr<char[]> texWidthStringBuffer( new char[texWidthStringLength + 2] );
		fgets( texWidthStringBuffer.get(), texWidthStringLength + 1, file );
		texWidthStringBuffer[texWidthStringLength + 1] = '\0';
		std::string texWidthString( texWidthStringBuffer.get() );
		int texWidth = std::stoi( texWidthString );
		textureResource.width = texWidth;

		//deserialize texture height
		int texHeightStringLength = getc( file );
		std::unique_ptr<char[]> texHeightStringBuffer( new char[texHeightStringLength + 2] );
		fgets( texHeightStringBuffer.get(), texHeightStringLength + 1, file );
		texHeightStringBuffer[texHeightStringLength + 1] = '\0';
		std::string texHeightString( texHeightStringBuffer.get() );
		int texHeight = std::stoi( texHeightString );
		textureResource.height = texHeight;

		//deserialize texture channels
		int texChannels = getc( file );
		textureResource.channels = texChannels;

		//deserialize texture data size
		int texDataSizeStringLength = getc( file );
		std::unique_ptr<char[]> texDataSizeStringBuffer( new char[texDataSizeStringLength + 2] );
		fgets( texDataSizeStringBuffer.get(), texDataSizeStringLength + 1, file );
		texDataSizeStringBuffer[texDataSizeStringLength + 1] = '\0';
		std::string texDataSizeString( texDataSizeStringBuffer.get() );
		long texDataSize = std::stol( texDataSizeString );
		textureResource.dataSize = texDataSize;

		//deserialize texture data
		textureResource.data = new char[textureResource.dataSize];		
		fpos_t currentPos;
		fgetpos( file, &currentPos );
		fread( textureResource.data, 1, textureResource.dataSize, file );
		fpos_t newPosition = currentPos + textureResource.dataSize;
		fsetpos( file, &newPosition );

		textures[textureResource.localName] = textureResource;
	}
	fclose( file );
}

/**
* @brief releases memory for all data
*/
void TextureResourceLoader::release()
{
	for( auto & token : textures )
	{
		delete[] token.second.data;
	}
	textures.clear();
}

/**
* @brief returns texture resource object for a given name
* @param textureName local name of the texture
*/
const TextureResource & TextureResourceLoader::getTextureResource( const std::string & textureName )
{
	const TextureResource & resource = textures.at( textureName );
	return resource;
}
