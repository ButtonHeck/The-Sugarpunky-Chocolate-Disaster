/*
 * Copyright 2019 Ilya Malgin
 * PlantGenerator.cpp
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
 * Purpose: contains definitions for PlantGenerator class
 * @version 0.1.0
 */

#include "PlantGenerator"
#include "Model"

#include <iomanip>
#include <chrono>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

/**
 * @brief sets seed for randomizer
 */
PlantGenerator::PlantGenerator() noexcept
{
	static bool randomizerInitialized = false;
	if( !randomizerInitialized )
	{
		randomizer.seed( std::chrono::system_clock::now().time_since_epoch().count() );
		randomizerInitialized = true;
	}
}

/**
 * @brief fills model chunks storage with empty chunks to work during next stages
 * @param map terrain map used to define height values of the chunk coordinates
 */
void PlantGenerator::initializeModelChunks( const map2D_f & map )
{
	//in case of reinitialization make sure to clear previous content
	chunks.clear();
	chunks.reserve( CHUNK_SIZE * CHUNK_SIZE );
	for( unsigned int y = 0; y < WORLD_HEIGHT; y += CHUNK_SIZE )
	{
		for( unsigned int x = 0; x < WORLD_WIDTH; x += CHUNK_SIZE )
		{
			chunks.emplace_back( x, x + CHUNK_SIZE, y, y + CHUNK_SIZE, glm::max( map[y][x], 0.0f ) );
		}
	}
}

/**
 * @brief setup chunks for actual rendering and update height values
 * @param map 2d map of the given terrain type
 */
void PlantGenerator::initializeModelRenderChunks( const map2D_f & map )
{
	//in case of reinitialization make sure to clear previous content
	renderChunks.clear();
	renderChunks.reserve( chunks.size() );
	for( auto& chunk : chunks )
	{
		bool isEmpty = true;
		for( auto numInstances : chunk.getNumInstancesVector() )
		{
			if( numInstances != 0 )
			{
				isEmpty = false;
				break;
			}
		}
		if( !isEmpty )
		{
			renderChunks.push_back( chunk );
		}
	}
	renderChunks.shrink_to_fit();

	for( ModelChunk & renderChunk : renderChunks )
	{
		renderChunk.setHeight( glm::max( 0.0f, map[renderChunk.getTop()][renderChunk.getLeft()] ) );
	}
}

/**
 * @brief performs serialization of generated data
 * @param output file stream for serialization
 */
void PlantGenerator::serialize( std::ofstream & output )
{
	for( unsigned int chunk = 0; chunk < chunks.size(); chunk++ )
	{
		//serialize number of models instances
		for( unsigned int modelIndex = 0; modelIndex < chunks[chunk].getNumInstancesVector().size(); )
		{
			//batching similar results for multiple models (i.e. if no models instances appear to be placed in this chunk)
			if( chunks[chunk].getNumInstances( modelIndex ) == 0 )
			{
				unsigned int zeroesInRow = 0;
				while( modelIndex < chunks[chunk].getNumInstancesVector().size() && chunks[chunk].getNumInstances( modelIndex ) == 0 )
				{
					zeroesInRow++;
					modelIndex++;
				}
				output << 0 << " " << zeroesInRow << " ";
			}
			//otherwise a particular model instance(s) has been placed in this chunk
			else
			{
				output << chunks[chunk].getNumInstances( modelIndex ) << " ";
				modelIndex++;
			}
		}

		//serialize models instances offsets for this chunk
		for( unsigned int modelIndex = 0; modelIndex < chunks[chunk].getInstanceOffsetVector().size(); )
		{
			//same batching routine for offsets here
			if( chunks[chunk].getInstanceOffset( modelIndex ) == 0 )
			{
				unsigned int zeroesInRow = 0;
				while( modelIndex < chunks[chunk].getInstanceOffsetVector().size() && chunks[chunk].getInstanceOffset( modelIndex ) == 0 )
				{
					zeroesInRow++;
					modelIndex++;
				}
				output << 0 << " " << zeroesInRow << " ";
			}
			else
			{
				output << chunks[chunk].getInstanceOffset( modelIndex ) << " ";
				modelIndex++;
			}
		}
	}

	//for each model's each instance serialize its matrix
	for( unsigned int modelIndex = 0; modelIndex < matrices.size(); modelIndex++ )
	{
		output << numPlants[modelIndex] << " ";
		for( unsigned int instanceIndex = 0; instanceIndex < numPlants[modelIndex]; instanceIndex++ )
		{
			float * matrixValues = (float*)glm::value_ptr( matrices[modelIndex][instanceIndex] );
			//precision cut down to preserve memory
			output << std::setprecision( 4 );
			for( unsigned int e = 0; e < 16; ++e )
			{
				output << matrixValues[e] << " ";
			}
		}
	}
}

/**
 * @brief performs deserialization for datum kept in a given file stream
 * @param input file stream to read data from
 */
void PlantGenerator::deserialize( std::ifstream & input )
{
	for( unsigned int chunk = 0; chunk < chunks.size(); chunk++ )
	{
		//load number of models instances
		for( unsigned int modelIndex = 0; modelIndex < chunks[chunk].getNumInstancesVector().size(); )
		{
			unsigned int numInstances;
			input >> numInstances;
			//unfolding similar results for multiple models (i.e. if no models instances appear to be placed in this chunk)
			if( numInstances == 0 )
			{
				unsigned int zeroesInRow;
				input >> zeroesInRow;
				for( unsigned int neighbourModelIndex = modelIndex; neighbourModelIndex < modelIndex + zeroesInRow; neighbourModelIndex++ )
				{
					chunks[chunk].setNumInstances( neighbourModelIndex, 0 );
				}
				modelIndex += zeroesInRow;
			}
			//otherwise a particular model instance(s) has been placed in this chunk
			else
			{
				chunks[chunk].setNumInstances( modelIndex, numInstances );
				modelIndex++;
			}
		}

		//load models instances offsets for this chunk
		for( unsigned int modelIndex = 0; modelIndex < chunks[chunk].getInstanceOffsetVector().size(); )
		{
			unsigned int offset;
			input >> offset;
			//same unfolding routine for offsets here
			if( offset == 0 )
			{
				unsigned int zeroesInRow;
				input >> zeroesInRow;
				for( unsigned int neighbourModelIndex = modelIndex; neighbourModelIndex < modelIndex + zeroesInRow; neighbourModelIndex++ )
				{
					chunks[chunk].setInstanceOffset( neighbourModelIndex, 0 );
				}
				modelIndex += zeroesInRow;
			}
			else
			{
				chunks[chunk].setInstanceOffset( modelIndex, offset );
				modelIndex++;
			}
		}
	}

	//loading all the instance matrices for each model
	map2D_mat4 newMatrices;
	for( unsigned int modelIndex = 0; modelIndex < matrices.size(); modelIndex++ )
	{
		unsigned int numPlantsForCurrentModel = 0;
		input >> numPlantsForCurrentModel;
		newMatrices.emplace_back( std::vector<glm::mat4>( numPlantsForCurrentModel ) );
		for( unsigned int instanceIndex = 0; instanceIndex < numPlantsForCurrentModel; instanceIndex++ )
		{
			glm::mat4 model;
			float * modelData = (float*)glm::value_ptr( model );
			for( unsigned int e = 0; e < 16; e++ )
			{
				input >> modelData[e];
			}
			newMatrices[modelIndex][instanceIndex] = std::move( model );
		}
	}
	//update loaded matrices
	loadMatrices( newMatrices );
}

/**
 * @brief for each model prepare its indirect buffer data using CPU frustum culling
 * @param cameraPositionXZ X and Z coordinates of a current view position
 * @param viewFrustum frustum to perform CPU culling
 */
void PlantGenerator::prepareIndirectBufferData( const glm::vec2 & cameraPositionXZ, 
												const Frustum & viewFrustum )
{
	//firstly precalculate only those chunks that are visible in a view frustum and close enough to a camera
	std::vector<std::pair<ModelChunk, unsigned int>> visibleChunks;
	visibleChunks.reserve( NUM_CHUNKS / 2 );

	for( ModelChunk & chunk : renderChunks )
	{
		if( chunk.isInsideFrustum( viewFrustum, cullingOffset ) )
		{
			glm::vec2 directionToChunkCenter = chunk.getMidPoint() - cameraPositionXZ;
			unsigned int distanceToChunk = glm::length2( directionToChunkCenter );

			//if a chunk is farther than the low-poly render distance - just discard it
			if( distanceToChunk < LOADING_DISTANCE_UNITS_LOWPOLY_SQUARE )
			{
				visibleChunks.emplace_back( chunk, distanceToChunk );
			}
		}
	}

	for( unsigned int modelIndex = 0; modelIndex < models.size(); modelIndex++ )
	{
		Model & model = models[modelIndex];
		model.prepareIndirectBufferData( visibleChunks, modelIndex, LOADING_DISTANCE_UNITS_SQUARE, LOADING_DISTANCE_UNITS_SHADOW_SQUARE );
		Model & lowPolyModel = lowPolyModels[modelIndex];
		lowPolyModel.prepareIndirectBufferData( visibleChunks, modelIndex, LOADING_DISTANCE_UNITS_SQUARE, LOADING_DISTANCE_UNITS_SHADOW_SQUARE );
	}
}

/**
 * @brief for each model upload data kept in indirect buffer from CPU to GPU
 */
void PlantGenerator::updateIndirectBufferData()
{
	for( unsigned int modelIndex = 0; modelIndex < models.size(); modelIndex++ )
	{
		Model & model = models[modelIndex];
		model.updateIndirectBufferData();
		Model & lowPolyModel = lowPolyModels[modelIndex];
		lowPolyModel.updateIndirectBufferData();
	}
}

/**
 * @brief update matrices storage data and load all instance matrices for each model accordingly (both plain and low-poly)
 * @param newMatrices storage for instance matrices
 */
void PlantGenerator::loadMatrices( const map2D_mat4 & newMatrices )
{
	//in case of reallocation reinitialize total numbers of each model and its instance matrices
	numPlants.reset( new unsigned int[newMatrices.size()] );
	matrices.clear();
	matrices.reserve( newMatrices.size() );

	//update generator's internal storages
	for( unsigned int modelIndex = 0; modelIndex < newMatrices.size(); modelIndex++ )
	{
		matrices.emplace_back( std::vector<glm::mat4>( newMatrices[modelIndex].size() ) );
		for( unsigned int instanceIndex = 0; instanceIndex < newMatrices[modelIndex].size(); instanceIndex++ )
		{
			matrices[modelIndex][instanceIndex] = newMatrices[modelIndex][instanceIndex];
		}
		numPlants[modelIndex] = newMatrices[modelIndex].size();
	}

	//update instance matrices for each provided model
	for( unsigned int modelIndex = 0; modelIndex < models.size(); modelIndex++ )
	{
		models[modelIndex].loadModelInstances( matrices[modelIndex] );
	}
	for( unsigned int modelIndex = 0; modelIndex < lowPolyModels.size(); modelIndex++ )
	{
		lowPolyModels[modelIndex].loadModelInstances( matrices[modelIndex] );
	}
}

/**
 * @brief clears current matrices storage and gives a boilerplate one for future allocation instead
 * @return empty storage to work with
 */
map2D_mat4 PlantGenerator::substituteMatricesStorage()
{
	map2D_mat4 newMatrices;
	newMatrices.reserve( models.size() );
	for( unsigned int modelIndex = 0; modelIndex < models.size(); modelIndex++ )
	{
		newMatrices.emplace_back( std::vector<glm::mat4>() );
	}
	matrices.clear();
	return newMatrices;
}

std::vector<Model> & PlantGenerator::getModels( bool isLowPoly ) noexcept
{
	return isLowPoly ? lowPolyModels : models;
}

std::vector<ModelChunk> & PlantGenerator::getChunks() noexcept
{
	return chunks;
}
