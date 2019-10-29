/*
 * Copyright 2019 Ilya Malgin
 * Model.cpp
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
 * Purpose: contains definition for Model class
 * @version 0.1.0
 */

#include "Model"
#include "BindlessTextureManager"
#include "TextureLoader"
#include "ModelResourceLoader"

#include <string>

TextureLoader * Model::textureLoader;
void Model::bindTextureLoader( TextureLoader & textureLoader ) noexcept
{
	Model::textureLoader = &textureLoader;
}

/**
 * @brief plain ctor
 * @param localName model's relative path and name
 * @param isLowPoly defines whether this model would be approached as low-poly
 * @param numRepetitions defines how many times in a row this model would be used during allocation on the map
 * @param isInstanced defines whether this model would be rendered with instancing
 */
Model::Model( const std::string & localName,
			  bool isLowPoly,
			  unsigned int numRepetitions,
			  bool isInstanced )
	: isInstanced( isInstanced )
	, isLowPoly( isLowPoly )
	, numRepetitions( numRepetitions )
	, GPUDataManager( isLowPoly )
	, renderer( GPUDataManager.getBasicGLBuffers(), GPUDataManager.getDepthmapDIBO(), GPUDataManager.getReflectionDIBO() )
{
	load( localName );
}

/**
 * @brief parses model resource data to own data storage
 * @param localName model's relative path and name
 */
void Model::load( const std::string & localName )
{
	const ModelResource & resource = ModelResourceLoader::getModelResource( localName );

	//parse textures
	loadTextures( resource );

	GPUDataManager.setupBuffers( resource.verticesData, resource.numVertices, resource.indicesData, resource.numIndices, isInstanced );
}

/**
* @brief helper function to parse and load model textures 
* @param resource model resource
*/
void Model::loadTextures( const ModelResource & resource )
{
	//load diffuse textures
	for( int dTextureIndex = 0; dTextureIndex < resource.numDiffuseTextures; dTextureIndex++ )
	{
		const std::string & TEXTURE_NAME = resource.diffuseTextures[dTextureIndex].localName;
		GLuint texture = textureLoader->loadTexture( TEXTURE_NAME.c_str(), 0, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true, true, false );
		std::string textureUniformName( "u_textureDiffuse[" );
		textureUniformName.append( std::to_string( resource.diffuseTextures[dTextureIndex].samplerIndex )).append( "]" );
		BindlessTextureManager::emplaceBack( textureUniformName, texture, BINDLESS_TEXTURE_MODEL );
	}
	//load specular textures
	for( int sTextureIndex = 0; sTextureIndex < resource.numSpecularTextures; sTextureIndex++ )
	{
		const std::string & TEXTURE_NAME = resource.specularTextures[sTextureIndex].localName;
		GLuint texture = textureLoader->loadTexture( TEXTURE_NAME.c_str(), 0, GL_REPEAT, GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, true, true, true );
		std::string textureUniformName( "u_textureSpecular[" );
		textureUniformName.append( std::to_string( resource.specularTextures[sTextureIndex].samplerIndex ) ).append( "]" );
		BindlessTextureManager::emplaceBack( textureUniformName, texture, BINDLESS_TEXTURE_MODEL );
	}
}

/**
 * @brief delegates a draw call to renderer
 * @param isShadow define whether depthmap or plain on-screen rendering mode is on
 */
void Model::draw( bool isShadow )
{
	MODEL_INDIRECT_BUFFER_TYPE type = isShadow ? DEPTHMAP_OFFSCREEN : PLAIN_ONSCREEN;
	renderer.render( type, GPUDataManager.getPrimitiveCount( type ) );
}

/**
* @brief delegates world reflection call to renderer
*/
void Model::drawWorldReflection()
{
	renderer.render( REFLECTION_ONSCREEN, GPUDataManager.getPrimitiveCount( REFLECTION_ONSCREEN ) );
}

void Model::drawOneInstance()
{
	renderer.renderOneInstance( GPUDataManager.getIndicesCount() );
}

/**
 * @brief delegates indirect buffer data preparation to model's data manager
 * @param visibleChunks model's chunks storage with corresponding distance from the camera position
 * @param modelIndex model's index in a chunks storage
 * @param loadingDistance rendering distance for full-res models
 * @param loadingDistanceShadow rendering distance for models' shadows
 */
void Model::prepareIndirectBufferData( const std::vector<std::pair<ModelChunk, unsigned int>> & visibleChunks,
									   unsigned int modelIndex,
									   float loadingDistance,
									   float loadingDistanceShadow )
{
	GPUDataManager.prepareIndirectBufferData( visibleChunks, modelIndex, loadingDistance, loadingDistanceShadow );
}

/**
 * @brief delegates indirect buffer data GPU update to model's data manager
 */
void Model::updateIndirectBufferData()
{
	GPUDataManager.updateIndirectBufferData();
}

/**
 * @brief delegates model's instances data loading to data manager
 * @param instanceMatrices instance matrices storage
 */
void Model::loadModelInstances( const std::vector<glm::mat4> & instanceMatrices )
{
	GPUDataManager.loadModelInstancesData( instanceMatrices );
}

unsigned int Model::getRepeatCount() const noexcept
{
	return numRepetitions;
}
