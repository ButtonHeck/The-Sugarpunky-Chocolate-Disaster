/*
 * Copyright 2019 Ilya Malgin
 * FontLoader.h
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
 * Purpose: contains declaration for FontLoader class and Character struct
 * @version 0.1.0
 */

#pragma once

#include <GL/glew.h>
#include <unordered_map>

/**
* @brief representation of a plain text symbol
*/
struct Character
{
	int asciiCode;
	int x;
	int y;
	int width;
	int height;
	int xoffset;
	int yoffset;
	int xadvance;
};

/**
* @brief font handling class. Responsible for creating an alphabet of Character objects from given files
*/
class FontLoader
{
public:
	FontLoader( const std::string & fontFile, 
				const std::string & fontTexture );
	std::unordered_map<char, Character> & getAlphabet() noexcept;
	GLuint getTextureWidth() const noexcept;
	GLuint getTextureHeight() const noexcept;

private:
	std::unordered_map<char, Character> alphabet;
	GLuint fontTexture;
	GLuint textureWidth;
	GLuint textureHeight;
};
