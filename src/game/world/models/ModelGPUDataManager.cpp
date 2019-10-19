/*
 * Copyright 2019 Ilya Malgin
 * ModelGPUDataManager.cpp
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
 * Purpose: contains definitions for ModelGPUDataManager class and IndirectBufferToken struct
 * @version 0.1.0
 */

#include "ModelGPUDataManager"
#include "BenchmarkTimer"
#include "ModelChunk"
#include "ModelVertex"
#include "SceneSettings"

/**
* @brief plain ctor
* @param isParentModelLowPoly indicator of the model's low-poly flag
*/
ModelGPUDataManager::ModelGPUDataManager( bool isParentModelLowPoly )
	: isLowPoly( isParentModelLowPoly )
	, basicGLBuffers( VAO | VBO | INSTANCE_VBO | EBO )
{}

/**
* @brief setup OpenGL buffers state and buffer necessary data to GPU
* @param vertices storage with vertices data
* @param indices storage with indices data
* @param useIndirectBuffer indicator of whether indirect buffer will be used for this model
*/
void ModelGPUDataManager::setupBuffers( const std::vector<ModelVertex> & vertices,
										const std::vector<GLuint> & indices, 
										bool useIndirectBuffer )
{
	indicesCount = indices.size();
	basicGLBuffers.bind( VAO | VBO | EBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( ModelVertex ) * vertices.size(), &vertices[0], GL_STATIC_DRAW );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( GLuint ) * indicesCount, &indices[0], GL_STATIC_DRAW );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( ModelVertex ), (void*)0 );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( ModelVertex ), (void*)offsetof( ModelVertex, Normal ) );
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( ModelVertex ), (void*)offsetof( ModelVertex, TexCoords ) );
	glEnableVertexAttribArray( 3 );
	glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, sizeof( ModelVertex ), (void*)offsetof( ModelVertex, Tangent ) );
	glEnableVertexAttribArray( 4 );
	glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE, sizeof( ModelVertex ), (void*)offsetof( ModelVertex, Bitangent ) );
	glEnableVertexAttribArray( 9 );
	//intentionally set GL_FLOAT although the data is a pair of unsigned integers
	glVertexAttribPointer( 9, 2, GL_FLOAT, GL_FALSE, sizeof( ModelVertex ), (void*)offsetof( ModelVertex, TexIndices ) );

	if( useIndirectBuffer )
	{
		setupIndirectBuffers();
	}

	BufferCollection::bindZero( VAO | VBO | EBO );
}

/**
* @brief initializes indirect buffer for plain onscreen, depthmap and world reflection buffer collections
*/
void ModelGPUDataManager::setupIndirectBuffers()
{
	GLuint numElements = NUM_CHUNKS * INDIRECT_DRAW_COMMAND_ARGUMENTS;
	GLsizei sizeInBytes = sizeof( GLuint ) * numElements;
	if( basicGLBuffers.get( DIBO ) == 0 )
	{
		multiDrawIndirectData = std::make_unique<GLuint[]>( numElements );
		basicGLBuffers.add( DIBO );
		basicGLBuffers.bind( DIBO );
		glNamedBufferStorage( basicGLBuffers.get( DIBO ), sizeInBytes, 0, GL_DYNAMIC_STORAGE_BIT );
	}
	if( depthmapDIBO.get( DIBO ) == 0 )
	{
		multiDrawIndirectDataDepthmap = std::make_unique<GLuint[]>( numElements );
		depthmapDIBO.add( DIBO );
		depthmapDIBO.bind( DIBO );
		glNamedBufferStorage( depthmapDIBO.get( DIBO ), sizeInBytes, 0, GL_DYNAMIC_STORAGE_BIT );
	}
	if( reflectionDIBO.get( DIBO ) == 0 )
	{
		multiDrawIndirectDataReflection = std::make_unique<GLuint[]>( numElements );
		reflectionDIBO.add( DIBO );
		reflectionDIBO.bind( DIBO );
		glNamedBufferStorage( reflectionDIBO.get( DIBO ), sizeInBytes, 0, GL_DYNAMIC_STORAGE_BIT );
	}
}

/**
* @brief updates internal indirect buffer storages with given chunks data
* @param visibleChunks storage of the currently visible models chunks
* @param modelIndex index of this model in each chunk
* @param loadingDistance maximum distance for onscreen rendering of full-poly models
* @param loadingDistanceShadow maximum distance for offscreen depthmap rendering of all models
*/
void ModelGPUDataManager::prepareIndirectBufferData( const std::vector<std::pair<ModelChunk, unsigned int>> & visibleChunks,
													 unsigned int modelIndex,
													 float loadingDistance,
													 float loadingDistanceShadow )
{
	BENCHMARK( "(ST)Model: prepare indirect buffer", true );
	//recreate indirect draw commands storages
	indirectTokens.clear();
	indirectTokens.reserve( visibleChunks.size() );
	indirectTokensDepthmap.clear();
	indirectTokensDepthmap.reserve( visibleChunks.size() );
	indirectTokensReflection.clear();
	indirectTokensReflection.reserve( visibleChunks.size() );

	for( unsigned int chunkIndex = 0; chunkIndex < visibleChunks.size(); chunkIndex++ )
	{
		const ModelChunk & chunk = visibleChunks[chunkIndex].first;
		unsigned int distanceToChunk = visibleChunks[chunkIndex].second;

		unsigned int numInstancesInChunk = chunk.getNumInstances( modelIndex );
		//no need to process chunk if this model is not presented in there
		if( numInstancesInChunk == 0 )
		{
			continue;
		}
		unsigned int instanceOffsetInChunk = chunk.getInstanceOffset( modelIndex );

		if( distanceToChunk < loadingDistanceShadow )
		{
			if( !isLowPoly )
			{
				if( distanceToChunk < loadingDistance && !chunk.isOccluded() )
				{
					//draw nearby camera if fullpoly
					addIndirectBufferToken( numInstancesInChunk, instanceOffsetInChunk, PLAIN_ONSCREEN );
				}
			}
			else
			{
				//draw as shadow source if lowpoly
				addIndirectBufferToken( numInstancesInChunk, instanceOffsetInChunk, DEPTHMAP_OFFSCREEN );
			}
		}
		//draw farther from camera if lowpoly
		if( isLowPoly && distanceToChunk >= loadingDistance && !chunk.isOccluded() )
		{
			addIndirectBufferToken( numInstancesInChunk, instanceOffsetInChunk, PLAIN_ONSCREEN );
		}

		//for world reflection rendering mode better render all models as lowpoly regardless of distance 
		if( isLowPoly && !chunk.isOccluded() )
		{
			addIndirectBufferToken( numInstancesInChunk, instanceOffsetInChunk, REFLECTION_ONSCREEN );
		}
	}

	//after indirect tokens have been updated load them to local buffers
	GLuint dataOffset = 0;
	for( const auto & token : indirectTokens )
	{
		multiDrawIndirectData[dataOffset++] = indicesCount;
		multiDrawIndirectData[dataOffset++] = token.numInstances;
		multiDrawIndirectData[dataOffset++] = token.FIRST_INDEX;
		multiDrawIndirectData[dataOffset++] = token.BASE_VERTEX;
		multiDrawIndirectData[dataOffset++] = token.instanceOffset;
	}
	drawIndirectCommandPrimCount = indirectTokens.size();

	dataOffset = 0;
	for( const auto & token : indirectTokensDepthmap )
	{
		multiDrawIndirectDataDepthmap[dataOffset++] = indicesCount;
		multiDrawIndirectDataDepthmap[dataOffset++] = token.numInstances;
		multiDrawIndirectDataDepthmap[dataOffset++] = token.FIRST_INDEX;
		multiDrawIndirectDataDepthmap[dataOffset++] = token.BASE_VERTEX;
		multiDrawIndirectDataDepthmap[dataOffset++] = token.instanceOffset;
	}
	drawIndirectCommandPrimCountDepthmap = indirectTokensDepthmap.size();

	dataOffset = 0;
	for( const auto & token : indirectTokensReflection )
	{
		multiDrawIndirectDataReflection[dataOffset++] = indicesCount;
		multiDrawIndirectDataReflection[dataOffset++] = token.numInstances;
		multiDrawIndirectDataReflection[dataOffset++] = token.FIRST_INDEX;
		multiDrawIndirectDataReflection[dataOffset++] = token.BASE_VERTEX;
		multiDrawIndirectDataReflection[dataOffset++] = token.instanceOffset;
	}
	drawIndirectCommandPrimCountReflection = indirectTokensReflection.size();
}

/**
* @brief updates indirect buffers on the GPU side with data from client buffer storages
*/
void ModelGPUDataManager::updateIndirectBufferData()
{
	BENCHMARK( "Model: load indirect data to GPU", true );
	basicGLBuffers.bind( DIBO );
	glBufferSubData( GL_DRAW_INDIRECT_BUFFER, 0, INDIRECT_DRAW_COMMAND_BYTE_SIZE * drawIndirectCommandPrimCount, multiDrawIndirectData.get() );
	depthmapDIBO.bind( DIBO );
	glBufferSubData( GL_DRAW_INDIRECT_BUFFER, 0, INDIRECT_DRAW_COMMAND_BYTE_SIZE * drawIndirectCommandPrimCountDepthmap, multiDrawIndirectDataDepthmap.get() );
	reflectionDIBO.bind( DIBO );
	glBufferSubData( GL_DRAW_INDIRECT_BUFFER, 0, INDIRECT_DRAW_COMMAND_BYTE_SIZE * drawIndirectCommandPrimCountReflection, multiDrawIndirectDataReflection.get() );
}

/**
* @brief updates instances dedicated vbo with given data
* @param instanceMatrices storage of instances 'model' matrices
*/
void ModelGPUDataManager::loadModelInstancesData( const std::vector<glm::mat4> & instanceMatrices )
{
	basicGLBuffers.bind( VAO );
	if( basicGLBuffers.get( INSTANCE_VBO ) != 0 )
	{
		basicGLBuffers.bind( INSTANCE_VBO );
		glInvalidateBufferData( basicGLBuffers.get( INSTANCE_VBO ) );
		basicGLBuffers.deleteBuffer( INSTANCE_VBO );
	}
	basicGLBuffers.add( INSTANCE_VBO );
	basicGLBuffers.bind( INSTANCE_VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( glm::mat4 ) * instanceMatrices.size(), &instanceMatrices[0], GL_STATIC_DRAW );
	for( unsigned int i = 0; i < 4; ++i )
	{
		glEnableVertexAttribArray( i + 5 );
		glVertexAttribPointer( i + 5, 4, GL_FLOAT, GL_FALSE, sizeof( glm::mat4 ), (void*)( i * sizeof( glm::vec4 ) ) );
		glVertexAttribDivisor( i + 5, 1 );
	}
	BufferCollection::bindZero( VAO | VBO );
}

/**
* @brief helper function that creates one indirect draw command token with given parameters
* @param numInstances number of instances for indirect draw command
* @param instanceOffset instance offset for indirect draw command
* @param type type of indirect buffer to which token should be added
*/
void ModelGPUDataManager::addIndirectBufferToken( GLuint numInstances, 
												  GLuint instanceOffset, 
												  MODEL_INDIRECT_BUFFER_TYPE type )
{
	if( type == PLAIN_ONSCREEN )
	{
		indirectTokens.emplace_back( numInstances, instanceOffset );
	}
	else if( type == DEPTHMAP_OFFSCREEN )
	{
		indirectTokensDepthmap.emplace_back( numInstances, instanceOffset );
	}
	else if( type == REFLECTION_ONSCREEN )
	{
		indirectTokensReflection.emplace_back( numInstances, instanceOffset );
	}
}

/**
* @brief reduces total number of indirect buffer tokens by merging adjacent ones into big one
* @param tokens collection of tokens
* @note unused, but kept for possible usage in a future
*/
void ModelGPUDataManager::reduceIndirectBufferTokens( std::vector<IndirectBufferToken> & tokens )
{
	std::vector<IndirectBufferToken> reduced;
	reduced.reserve( tokens.size() );
	for( unsigned int tokenIndex = 0; tokenIndex < tokens.size(); )
	{
		IndirectBufferToken & token = tokens[tokenIndex];
		GLuint tokenNumInstances = token.numInstances;
		GLuint tokenInstanceOffset = token.instanceOffset;
		unsigned int tokenIndexOffset = 1;
		while( tokenIndex + tokenIndexOffset < tokens.size() )
		{
			IndirectBufferToken & adjacentToken = tokens[tokenIndex + tokenIndexOffset];
			if( adjacentToken.instanceOffset == tokenInstanceOffset + tokenNumInstances )
			{
				tokenNumInstances += adjacentToken.numInstances;
				tokenInstanceOffset += adjacentToken.instanceOffset;
				++tokenIndexOffset;
			}
			else
			{
				break;
			}
		}
		reduced.emplace_back( tokenNumInstances, token.instanceOffset );
		tokenIndex += tokenIndexOffset;
	}
	tokens.assign( reduced.begin(), reduced.end() );
}

GLsizei ModelGPUDataManager::getPrimitiveCount( MODEL_INDIRECT_BUFFER_TYPE type ) const noexcept
{
	if( type == PLAIN_ONSCREEN )
	{
		return drawIndirectCommandPrimCount;
	}
	else if( type == DEPTHMAP_OFFSCREEN )
	{
		return drawIndirectCommandPrimCountDepthmap;
	}
	else
	{
		return drawIndirectCommandPrimCountReflection;
	}
}

GLuint ModelGPUDataManager::getIndicesCount() const noexcept
{
	return indicesCount;
}

BufferCollection & ModelGPUDataManager::getBasicGLBuffers() noexcept
{
	return basicGLBuffers;
}

BufferCollection & ModelGPUDataManager::getDepthmapDIBO() noexcept
{
	return depthmapDIBO;
}

BufferCollection & ModelGPUDataManager::getReflectionDIBO() noexcept
{
	return reflectionDIBO;
}

/**
* @brief plain ctor
* @param numInstances number of instances used in an indirect draw command
* @param instanceOffset instance offset used in an indirect draw command
*/
ModelGPUDataManager::IndirectBufferToken::IndirectBufferToken( GLuint numInstances, 
															   GLuint instanceOffset ) noexcept
	: numInstances( numInstances )
	, instanceOffset( instanceOffset )
{}
