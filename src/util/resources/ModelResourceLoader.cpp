/*
 * Copyright 2019 Ilya Malgin
 * ModelResourceLoader.cpp
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
 * Purpose: contains definitions for ModelResourceLoader class
 * @version 0.1.0
 */

#include "ModelResourceLoader"
#include "ModelVertex"

#include <fstream>

/**
* @brief storage for models resources
*/
std::unordered_map<std::string, ModelResource> ModelResourceLoader::models;

/**
* @brief loads data from .sprd file and creates model resources based on parsed data
* @param path name of the models .sprd file
*/
void ModelResourceLoader::initialize( const char * path )
{
	std::ifstream file( path, std::ios::binary );

	//read number of models
	int numModels = 0;
	file.read( reinterpret_cast<char*>( &numModels ), sizeof( numModels ) );
	models.reserve( numModels );

	//read models one by one
	for( int modelIndex = 0; modelIndex < numModels; modelIndex++ )
	{
		deserializeModel( file );
	}

	file.close();
}

/**
* @brief helper function to deserialize single model out of the file stream
* @param file file stream to read data from
*/
void ModelResourceLoader::deserializeModel( std::ifstream & file )
{
	ModelResource modelResource;

	//read name length
	int nameLength = 0;
	file.read( reinterpret_cast<char*>( &nameLength ), sizeof( nameLength ) );

	//read name
	std::unique_ptr<char[]> nameBuffer( new char[nameLength + 1] );
	file.read( nameBuffer.get(), nameLength );
	nameBuffer[nameLength] = '\0';
	std::string name( nameBuffer.get() );
	modelResource.localName = name;

	//read vertices count
	int verticesCount = 0;
	file.read( reinterpret_cast<char*>( &verticesCount ), sizeof( verticesCount ) );
	modelResource.numVertices = verticesCount;
	//read vertices data
	size_t vertexSize = sizeof( ModelVertex );
	int verticesNumBytes = vertexSize * verticesCount;
	modelResource.verticesData = new char[verticesNumBytes];
	file.read( modelResource.verticesData, verticesNumBytes );

	//read indices count
	int indicesCount = 0;
	file.read( reinterpret_cast<char*>( &indicesCount ), sizeof( indicesCount ) );
	modelResource.numIndices = indicesCount;
	//read indices data
	size_t indexSize = sizeof( unsigned int );
	int indicesNumBytes = indexSize * indicesCount;
	modelResource.indicesData = new char[indicesNumBytes];
	file.read( modelResource.indicesData, indicesNumBytes );

	//read diffuse textures count
	int numDiffuseTextures = 0;
	file.read( reinterpret_cast<char*>( &numDiffuseTextures ), sizeof( numDiffuseTextures ) );
	modelResource.numDiffuseTextures = numDiffuseTextures;
	modelResource.diffuseTextures.reserve( numDiffuseTextures );
	//read diffuse textures
	for( int textureIndex = 0; textureIndex < numDiffuseTextures; textureIndex++ )
	{
		ModelResourceTextureData textureData;

		//read sampler index
		int samplerIndex = 0;
		file.read( reinterpret_cast<char*>( &samplerIndex ), sizeof( samplerIndex ) );
		textureData.samplerIndex = samplerIndex;

		//read texture name length
		int textureNameLength = 0;
		file.read( reinterpret_cast<char*>( &textureNameLength ), sizeof( textureNameLength ) );

		//read texture name
		std::unique_ptr<char[]> textureNameBuffer( new char[textureNameLength + 1] );
		file.read( textureNameBuffer.get(), textureNameLength );
		textureNameBuffer[textureNameLength] = '\0';
		std::string textureName( textureNameBuffer.get() );
		textureData.localName = textureName;
		modelResource.diffuseTextures.push_back( textureData );
	}

	//read specular textures count
	int numSpecularTextures = 0;
	file.read( reinterpret_cast<char*>( &numSpecularTextures ), sizeof( numSpecularTextures ) );
	modelResource.numSpecularTextures = numSpecularTextures;
	modelResource.specularTextures.reserve( numSpecularTextures );
	//read specular textures
	for( int textureIndex = 0; textureIndex < numSpecularTextures; textureIndex++ )
	{
		ModelResourceTextureData textureData;

		//read sampler index
		int samplerIndex = 0;
		file.read( reinterpret_cast<char*>( &samplerIndex ), sizeof( samplerIndex ) );
		textureData.samplerIndex = samplerIndex;

		//read texture name length
		int textureNameLength = 0;
		file.read( reinterpret_cast<char*>( &textureNameLength ), sizeof( textureNameLength ) );

		//read texture name
		std::unique_ptr<char[]> textureNameBuffer( new char[textureNameLength + 1] );
		file.read( textureNameBuffer.get(), textureNameLength );
		textureNameBuffer[textureNameLength] = '\0';
		std::string textureName( textureNameBuffer.get() );
		textureData.localName = textureName;
		modelResource.specularTextures.push_back( textureData );
	}

	models[modelResource.localName] = modelResource;
}

/**
* @brief releases memory for all data
*/
void ModelResourceLoader::release()
{
	for( auto & token : models )
	{
		delete[] token.second.verticesData;
		delete[] token.second.indicesData;
	}
	models.clear();
}

/**
* @brief return model resource object for a given name
* @param localName local name of the model
*/
const ModelResource & ModelResourceLoader::getModelResource( const std::string & localName )
{
	const ModelResource & resource = models.at( localName );
	return resource;
}
