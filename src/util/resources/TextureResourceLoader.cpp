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
#include <fstream>

/**
* @brief storage for the parsed texture resources
*/
std::unordered_map<std::string, TextureResource> TextureResourceLoader::textures;

/**
* @brief loads data from the .sprd file and creates separate texture resources based on the data
* @param path name of the textures .sprd file
*/
void TextureResourceLoader::initialize( const char * path )
{
	textures.reserve( 512 );
	std::ifstream file( path, std::ios::binary );

	//Plain textures deserialization
	int numPlainTextures = 0;
	file.read( reinterpret_cast<char*>( &numPlainTextures ), sizeof( numPlainTextures ) );
	for( int textureIndex = 0; textureIndex < numPlainTextures; textureIndex++ )
	{
		deserializeTexture( file );
	}

	//models textures deserialization
	int numModelTextures = 0;
	file.read( reinterpret_cast<char*>( &numModelTextures ), sizeof( numModelTextures ) );
	for( int textureIndex = 0; textureIndex < numModelTextures; textureIndex++ )
	{
		deserializeTexture( file );
	}

	file.close();
}

/**
* @brief helper function to deserialize single texture resource using file stream
* @param file file stream to read data from
*/
void TextureResourceLoader::deserializeTexture( std::ifstream & file )
{
	TextureResource textureResource;

	//deserialize texture local name length
	int textureNameLength = 0;
	file.read( reinterpret_cast<char*>( &textureNameLength ), sizeof( textureNameLength ) );

	//deserialize texture local name
	std::unique_ptr<char[]> nameBuffer( new char[textureNameLength + 1] );
	file.read( nameBuffer.get(), textureNameLength );;
	nameBuffer[textureNameLength] = '\0';
	std::string textureName( nameBuffer.get() );
	textureResource.localName = textureName;

	//deserialize texture width
	int texWidth = 0;
	file.read( reinterpret_cast<char*>( &texWidth ), sizeof( texWidth ) );
	textureResource.width = texWidth;

	//deserialize texture height
	int texHeight = 0;
	file.read( reinterpret_cast<char*>( &texHeight ), sizeof( texHeight ) );
	textureResource.height = texHeight;

	//deserialize texture channels
	int texChannels = 0;
	file.read( reinterpret_cast<char*>( &texChannels ), sizeof( texChannels ) );
	textureResource.channels = texChannels;

	//deserialize texture data size
	int texDataSize = 0;
	file.read( reinterpret_cast<char*>( &texDataSize ), sizeof( texDataSize ) );
	textureResource.dataSize = texDataSize;

	//deserialize texture data
	textureResource.data = new char[textureResource.dataSize];
	file.read( textureResource.data, textureResource.dataSize );

	textures[textureResource.localName] = textureResource;
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
