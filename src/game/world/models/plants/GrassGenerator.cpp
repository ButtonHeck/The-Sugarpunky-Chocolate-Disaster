/*
 * Copyright 2019 Ilya Malgin
 * GrassGenerator.cpp
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
 * Purpose: contains definitions for GrassGenerator class
 * @version 0.1.0
 */

#include "GrassGenerator"
#include "Model"
#include "SettingsManager"

#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief load both plain and low-poly models
 */
GrassGenerator::GrassGenerator() noexcept
	: PlantGenerator()
{
	models.reserve( 8 );
	models.emplace_back( "grass/grass1/grass1.obj", false );
	models.emplace_back( "grass/grass2/grass2.obj", false );
	models.emplace_back( "grass/grass3/grass3.obj", false );
	models.emplace_back( "grass/grass4/grass4.obj", false );
	models.emplace_back( "grass/grass5/grass5.obj", false );
	models.emplace_back( "grass/grass6/grass6.obj", false );

	lowPolyModels.reserve( 8 );
	lowPolyModels.emplace_back( "grass/grass1LP/grass1LP.obj", true );
	lowPolyModels.emplace_back( "grass/grass2LP/grass2LP.obj", true );
	lowPolyModels.emplace_back( "grass/grass3LP/grass3LP.obj", true );
	lowPolyModels.emplace_back( "grass/grass4LP/grass4LP.obj", true );
	lowPolyModels.emplace_back( "grass/grass5LP/grass5LP.obj", true );
	lowPolyModels.emplace_back( "grass/grass6LP/grass6LP.obj", true );

	assert( lowPolyModels.size() == models.size() );

	cullingOffset = 0.0f;
}

/**
 * @brief initialize models chunks and accomodate grass models on the world map based on input maps
 * @param landMap map of the land
 * @param hillMap map of the hills
 * @param distributionMap map filled with distribution seed values
 */
void GrassGenerator::setup( const map2D_f & landMap, 
							const map2D_f & hillMap, 
							const map2D_i & distributionMap )
{
	initializeModelChunks( landMap );
	setupMatrices( landMap, hillMap, distributionMap );
	initializeModelRenderChunks( landMap, APPROXIMATE_GRASS_CHUNK_HEIGHT );
}

/**
 * @brief calculates instance matrices for grass models and spreads them on world map
 * @param landMap map of the land
 * @param hillMap map of the hills
 * @param distributionMap map filled with distribution seed values
 */
void GrassGenerator::setupMatrices( const map2D_f & landMap, 
									const map2D_f & hillMap, 
									const map2D_i & distributionMap )
{
	const int PLANTS_DISTRIBUTION_FREQUENCY = SettingsManager::getInt( "SCENE", "plants_distribution_freq" );

    //get empty boilerplate storage to fill during (re)allocation
	map2D_mat4 matricesStorage = substituteMatricesStorage();
	std::uniform_real_distribution<float> modelSizeDistribution( MIN_SCALE, MAX_SCALE );

	size_t numberOfModels = models.size();
	std::vector<unsigned int> instanceOffsetsVector( numberOfModels, 0 );

	//used for circular indexing of a particular model/chunk
	unsigned int matrixCounter = 0, chunkCounter = 0;
	for( unsigned int startY = 0; startY < WORLD_HEIGHT; startY += CHUNK_SIZE )
	{
		for( unsigned int startX = 0; startX < WORLD_WIDTH; startX += CHUNK_SIZE )
		{
			std::vector<unsigned int> numInstancesVector( numberOfModels, 0 );
			//need this to be set before allocation on each subsequent chunk as it depends on the previous ones
			chunks.at( chunkCounter ).setInstanceOffsetsVector( instanceOffsetsVector );
			for( unsigned int y = startY; y < startY + CHUNK_SIZE; y++ )
			{
				for( unsigned int x = startX; x < startX + CHUNK_SIZE; x++ )
				{
					//check if there is land and no visible hills
					if( ( landMap[y][x] == 0 && landMap[y + 1][x + 1] == 0 && landMap[y + 1][x] == 0 && landMap[y][x + 1] == 0 ) &&
						!( hillMap[y][x] > -HILLS_OFFSET_Y || 
						   hillMap[y + 1][x + 1] > -HILLS_OFFSET_Y || 
						   hillMap[y + 1][x] > -HILLS_OFFSET_Y || 
						   hillMap[y][x + 1] > -HILLS_OFFSET_Y ) &&
						rand() % ( PLANTS_DISTRIBUTION_FREQUENCY / 2 - 1 ) == 0 &&     //is there a randomizer "hit"
						distributionMap[y][x] > PLANTS_DISTRIBUTION_FREQUENCY / 2 )   //is a seed value at these coordinates high enough to proceed
					{
						glm::mat4 model;
						//offset on XZ to place on a tile center
						glm::vec3 translateVector( -HALF_WORLD_WIDTH_F + x + 0.5f, 0.0f, -HALF_WORLD_HEIGHT_F + y + 0.5f );
						model = glm::translate( model, translateVector );
						model = glm::rotate( model, glm::radians( (float)( rand() * WORLD_WIDTH + x * 5 ) ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
						glm::vec3 scaleVector( modelSizeDistribution( randomizer ), modelSizeDistribution( randomizer ), modelSizeDistribution( randomizer ) );
						model = glm::scale( model, scaleVector );

						size_t currentModelIndex = matrixCounter % numberOfModels;
						matricesStorage[currentModelIndex].emplace_back( std::move( model ) );
						++numInstancesVector[currentModelIndex];
						++instanceOffsetsVector[currentModelIndex];
						++matrixCounter;
					}
				}
			}
			chunks.at( chunkCounter ).setNumInstancesVector( numInstancesVector );
			++chunkCounter;
		}
	}
	loadMatrices( matricesStorage );
}
