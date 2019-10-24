/*
 * Copyright 2019 Ilya Malgin
 * ModelGPUDataManager.h
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
 * Purpose: contains declaration for ModelGPUDataManager class and IndirectBufferToken struct
 * @version 0.1.0
 */

#pragma once

#include "BufferCollection"
#include "ModelIndirectBufferTypes"

#include <glm/mat4x4.hpp>
#include <memory>

class ModelChunk;

/**
* @brief manager for GPU model data. Responsible for initializing and updating data for indirect rendering of a model.
* Has a separate indirect draw buffer for depthmap rendering
*/
class ModelGPUDataManager
{
public:
	ModelGPUDataManager( bool isParentModelLowPoly );
	void setupBuffers( const char * vertices,
					   unsigned int numVertices,
					   const char * indices,
					   unsigned int indicesCount,
					   bool useIndirectBuffer );
	void prepareIndirectBufferData( const std::vector<std::pair<ModelChunk, unsigned int> > & chunks,
									unsigned int modelIndex,
									float loadingDistance,
									float loadingDistanceShadow );
	void updateIndirectBufferData();
	void loadModelInstancesData( const std::vector<glm::mat4> & instanceMatrices );
	GLsizei getPrimitiveCount( MODEL_INDIRECT_BUFFER_TYPE type ) const noexcept;
	GLuint getIndicesCount() const noexcept;
	BufferCollection & getBasicGLBuffers() noexcept;
	BufferCollection & getDepthmapDIBO() noexcept;
	BufferCollection & getReflectionDIBO() noexcept;

private:
	/**
	* @brief representation of one indirect draw command data token
	*/
	struct IndirectBufferToken
	{
		//{ indicesCount, numInstancesToDraw, firstIndex, baseVertex, baseInstance }
		IndirectBufferToken( GLuint numInstances, 
							 GLuint instanceOffset ) noexcept;

		GLuint numInstances;
		GLuint instanceOffset;
		static constexpr GLuint FIRST_INDEX = 0;
		static constexpr GLuint BASE_VERTEX = 0;
	};

	void setupIndirectBuffers();
	void addIndirectBufferToken( GLuint numInstances, 
								 GLuint instanceOffset, 
								 MODEL_INDIRECT_BUFFER_TYPE type );
	void reduceIndirectBufferTokens( std::vector<IndirectBufferToken> & tokens );

	//parent model attributes
	GLuint indicesCount;
	bool isLowPoly;

	//screen rendering related variables
	BufferCollection basicGLBuffers;
	std::unique_ptr<GLuint[]> multiDrawIndirectData;
	std::vector<IndirectBufferToken> indirectTokens;
	GLsizei drawIndirectCommandPrimCount = 0;

	//depthmap rendering related variables
	BufferCollection depthmapDIBO;
	std::unique_ptr<GLuint[]> multiDrawIndirectDataDepthmap;
	std::vector<IndirectBufferToken> indirectTokensDepthmap;
	GLsizei drawIndirectCommandPrimCountDepthmap = 0;

	//world reflection rendering related variables
	BufferCollection reflectionDIBO;
	std::unique_ptr<GLuint[]> multiDrawIndirectDataReflection;
	std::vector<IndirectBufferToken> indirectTokensReflection;
	GLsizei drawIndirectCommandPrimCountReflection = 0;
};
