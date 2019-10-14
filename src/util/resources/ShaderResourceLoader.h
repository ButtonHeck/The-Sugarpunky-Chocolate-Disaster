/*
 * Copyright 2019 Ilya Malgin
 * ShaderResourceLoader.h
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
 * Purpose: contains declaration for ShaderResourceLoader class and ShaderResource struct
 * @version 0.1.0
 */

#pragma once

#include <unordered_map>

/**
* @brief representation of a shader resource structure
*/
struct ShaderResource
{
	std::string localName;
	int type;
	size_t dataSize;
	char * data;
};

/**
* @brief utility class for loading shader sources data
*/
class ShaderResourceLoader
{
public:
	ShaderResourceLoader() = delete;
	static void initialize( const char * path );
	static void release();
	static const ShaderResource & getShaderResource( const std::string & shaderSourceName );

private:
	static std::unordered_map<std::string, ShaderResource> shaders;
};
