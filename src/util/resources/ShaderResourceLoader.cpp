/*
 * Copyright 2019 Ilya Malgin
 * ShaderResourceLoader.cpp
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
 * Purpose: contains definitions for ShaderResourceLoader class
 * @version 0.1.0
 */

#include "ShaderResourceLoader"

#include <string>
#include <fstream>

/**
* @brief storage for shader sources resources
*/
std::unordered_map<std::string, ShaderResource> ShaderResourceLoader::shaders;

/**
* @brief loads data from .sprd file and creates shader resources based on parsed data
* @param path name of the shader sources .sprd file
*/
void ShaderResourceLoader::initialize( const char * path )
{
	std::ifstream file( path, std::ios::binary );
	
	int numShaderSources = 0;
	file.read( reinterpret_cast<char*>( &numShaderSources ), sizeof( numShaderSources ) );
	shaders.reserve( numShaderSources );

	for( int shaderIndex = 0; shaderIndex < numShaderSources; shaderIndex++ )
	{
		ShaderResource resource;

		//deserialize name length
		int shaderNameLength = 0;
		file.read( reinterpret_cast<char*>( &shaderNameLength ), sizeof( shaderNameLength ) );

		//deserialize name
		std::unique_ptr<char[]> nameBuffer( new char[shaderNameLength + 1] );
		file.read( nameBuffer.get(), shaderNameLength );
		nameBuffer[shaderNameLength] = '\0';
		std::string shaderName( nameBuffer.get() );
		resource.localName = shaderName;

		//deserialize type
		int sourceType = 0;
		file.read( reinterpret_cast<char*>( &sourceType ), sizeof( sourceType ) );
		resource.type = sourceType;

		//deserialize shader source data size
		int dataSize = 0;
		file.read( reinterpret_cast<char*>( &dataSize ), sizeof( dataSize ) );
		resource.dataSize = dataSize;

		//deserialize shader source text
		resource.data = new char[resource.dataSize + 1];
		file.read( resource.data, resource.dataSize );
		resource.data[resource.dataSize] = '\0';

		shaders[resource.localName] = resource;
	}
	file.close();
}

/**
* @brief releases memory for all data
*/
void ShaderResourceLoader::release()
{
	for( auto & token : shaders )
	{
		delete[] token.second.data;
	}
	shaders.clear();
}

/**
* @brief returns shader resource object for a given name
* @param shaderSourceName local name of the shader source
*/
const ShaderResource & ShaderResourceLoader::getShaderResource( const std::string & shaderSourceName )
{
	const ShaderResource & resource = shaders.at( shaderSourceName );
	return resource;
}
