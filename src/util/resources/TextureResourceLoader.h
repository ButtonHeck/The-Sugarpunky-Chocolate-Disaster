/*
 * Copyright 2019 Ilya Malgin
 * TextureResourceLoader.h
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
 * Purpose: contains declaration for TextureResourceLoader class and TextureResource struct
 * @version 0.1.0
 */

#pragma once

#include <unordered_map>

/**
* @brief representation of the texture resource structure
*/
struct TextureResource
{
	std::string localName;
	int width;
	int height;
	int channels;
	size_t dataSize;
	char * data;
};

/**
* @brief utility class for loading textures data from .sprd file
*/
class TextureResourceLoader
{
public:
	TextureResourceLoader() = delete;
	static void initialize( const char * path );
	static void release();
	static const TextureResource & getTextureResource( const char * textureName );

private:
	static void deserializeTexture( std::ifstream & file );
	static std::unordered_map<std::string, TextureResource> textures;
};