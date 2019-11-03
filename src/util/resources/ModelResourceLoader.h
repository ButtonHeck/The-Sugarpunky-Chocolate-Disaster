/*
 * Copyright 2019 Ilya Malgin
 * ModelResourceLoader.h
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
 * Purpose: contains declaration for ModelResourceLoader class, ModelResource and ModelResourceTextureData structs
 * @version 0.1.0
 */

#pragma once

#include <unordered_map>

/**
* @brief model specific texture data token 
*/
struct ModelResourceTextureData
{
	std::string localName;
	int samplerIndex;
};

/**
* @brief representation of a model resource
*/
struct ModelResource
{
	std::string localName;
	int numVertices;
	char * verticesData;
	int numIndices;
	char * indicesData;
	int numDiffuseTextures;
	std::vector<ModelResourceTextureData> diffuseTextures;
	int numSpecularTextures;
	std::vector<ModelResourceTextureData> specularTextures;
};

/**
* @brief utility class for loading model sources data
*/
class ModelResourceLoader
{
public:
	ModelResourceLoader() = delete;
	static void initialize( const char * path );
	static void release();
	static const ModelResource & getModelResource( const char * localName );

private:
	static void deserializeModel( std::ifstream & file );
	static std::unordered_map<std::string, ModelResource> models;
};
