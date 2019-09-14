/*
 * Copyright 2019 Ilya Malgin
 * PlantsFacade.cpp
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
 * Purpose: contains definitions for PlantsFacade class
 * @version 0.1.0
 */

#include "PlantsFacade"
#include "Generator"
#include "BenchmarkTimer"
#include "Model"

/**
 * @param renderPhongShader compiled Phong shader program provided to a personal shader manager
 * @param renderGouraudShader compiled Gouraud shader program provided to a personal shader manager
 */
PlantsFacade::PlantsFacade( Shader & renderPhongShader, 
							Shader & renderGouraudShader ) noexcept
	: shaders( renderPhongShader, renderGouraudShader )
{}

/**
 * @brief initializes distribution map and delegates initialization command for generators
 * @param landMap map of the land
 * @param hillMap map of the hills
 * @param hillsNormalMap map of the hills normals
 */
void PlantsFacade::setup( const map2D_f & landMap, 
						  const map2D_f & hillMap, 
						  const map2D_vec3 & hillsNormalMap )
{
	prepareDistributionMap();
	landPlantsGenerator.setup( landMap, hillMap, distributionMap );
	grassGenerator.setup( landMap, hillMap, distributionMap );
	hillTreesGenerator.setup( hillMap, distributionMap, hillsNormalMap );
}

/**
 * @brief delegates render chunks update call to generators (necessary when loading terrain from file)
 * @param landMap map of the land
 * @param hillMap map of the hills
 */
void PlantsFacade::reinitializeModelRenderChunks( const map2D_f & landMap, 
												  const map2D_f & hillMap )
{
	landPlantsGenerator.initializeModelRenderChunks( landMap, APPROXIMATE_LAND_PLANTS_CHUNK_HEIGHT );
	grassGenerator.initializeModelRenderChunks( landMap, APPROXIMATE_GRASS_CHUNK_HEIGHT );
	hillTreesGenerator.initializeModelRenderChunks( hillMap, APPROXIMATE_HILL_PLANTS_CHUNK_HEIGHT );
}

/**
 * @brief for each generator's models delegates command to prepare its indirect buffer data on CPU side
 * @param camera player's camera
 * @param viewFrustum frustum to perform CPU culling
 * @param hillMap map of the hills
 */
void PlantsFacade::prepareIndirectBufferData( const Camera & camera, 
											  const Frustum & viewFrustum,
											  const map2D_f & hillMap )
{
	landPlantsGenerator.prepareIndirectBufferData( camera, viewFrustum, hillMap );
	hillTreesGenerator.prepareIndirectBufferData( camera, viewFrustum, hillMap );
	grassGenerator.prepareIndirectBufferData( camera, viewFrustum, hillMap );
}

/**
 * @brief for each generator's models delegates command to upload indirect buffer data from CPU to GPU
 */
void PlantsFacade::updateIndirectBufferData()
{
	BENCHMARK( "PlantsFacade: updateIndirectBuffer", true );
	landPlantsGenerator.updateIndirectBufferData();
	hillTreesGenerator.updateIndirectBufferData();
	grassGenerator.updateIndirectBufferData();
}

/**
 * @brief updates the shader program state, switches GL_BLEND mode if necessary and delegates draw calls to renderers
 * @param lightDir direction of the sunlight (directional lighting)
 * @param lightSpaceMatrices matrices for shadow sampling
 * @param projectionView Projection*View matrix
 * @param viewPosition current position of the camera
 * @param usePhongShading define what shading model to use
 * @param useShadows define whether to use shadows
 * @param useLandBlending define whether to use blending
 * @param worldReflectionMode define whether world reflection rendering stage is currently on
 */
void PlantsFacade::draw( const glm::vec3 & lightDir,
						 const std::array<glm::mat4, NUM_SHADOW_LAYERS> & lightSpaceMatrices,
						 const glm::mat4 & projectionView,
						 const glm::vec3 & viewPosition,
						 bool usePhongShading,
						 bool useShadows,
						 bool useLandBlending,
						 bool worldReflectionMode )
{
	if( useLandBlending && !worldReflectionMode )
	{
		glEnable( GL_BLEND );
	}
	else
	{
		glDisable( GL_BLEND );
	}

	shaders.updateAllPlants( usePhongShading,
							 lightDir,
							 lightSpaceMatrices,
							 projectionView,
							 viewPosition,
							 useShadows,
							 useLandBlending );

	//draw trees and hill models first (plain and low-poly)
	shaders.setType( PLANT_STATIC );
	if( !worldReflectionMode )
	{
		shaders.setLowPolyMode( false );
		treesRenderer.render( landPlantsGenerator.getModels( false ), hillTreesGenerator.getModels( false ), false );
		shaders.setLowPolyMode( true );
		treesRenderer.render( landPlantsGenerator.getModels( true ), hillTreesGenerator.getModels( true ), false );
	}
	else
	{
		shaders.setLowPolyMode( true );
		treesRenderer.renderWorldReflection( landPlantsGenerator.getModels( true ), hillTreesGenerator.getModels( true ) );
	}


	//draw grass (plain and low-poly), no need to render it if world reflection rendering stage is on
	if( !worldReflectionMode )
	{
		shaders.setType( PLANT_ANIMATED );
		shaders.updateGrassKeyframe();
		shaders.setLowPolyMode( false );
		grassRenderer.render( grassGenerator.getModels( false ), false );
		shaders.setLowPolyMode( true );
		grassRenderer.render( grassGenerator.getModels( true ), false );
	}

	if( useLandBlending || worldReflectionMode )
	{
		glDisable( GL_BLEND );
	}
}

/**
 * @brief performs shadow mode rendering process
 * @param grassCastShadow define whether grass would be drawn in shadow maps
 */
void PlantsFacade::drawDepthmap( bool grassCastShadow )
{
	treesRenderer.render( landPlantsGenerator.getModels( false ), hillTreesGenerator.getModels( false ), true );
	treesRenderer.render( landPlantsGenerator.getModels( true ), hillTreesGenerator.getModels( true ), true );
	if( grassCastShadow )
	{
		grassRenderer.render( grassGenerator.getModels( false ), true );
	}
}

/**
 * @brief delegates serialization command to generators
 * @param output file stream to write data to
 */
void PlantsFacade::serialize( std::ofstream & output )
{
	landPlantsGenerator.serialize( output );
	grassGenerator.serialize( output );
	hillTreesGenerator.serialize( output );
}

/**
 * @brief delegates deserialization command to generators
 * @param input file stream to read data from
 */
void PlantsFacade::deserialize( std::ifstream & input )
{
	landPlantsGenerator.deserialize( input );
	grassGenerator.deserialize( input );
	hillTreesGenerator.deserialize( input );
}

/**
 * @brief prepares distribution map used by generators during plants allocation
 */
void PlantsFacade::prepareDistributionMap()
{
	Generator::initializeMap( distributionMap );

	//for each subsequent cycle distribution kernel would be one unit wider in radius
	for( int cycle = 1; cycle <= PLANTS_DISTRIBUTION_FREQUENCY; cycle++ )
	{
		for( unsigned int startY = 0; startY < distributionMap.size(); startY++ )
		{
			for( unsigned int startX = 0; startX < distributionMap[0].size(); startX++ )
			{
				if( rand() % ( PLANTS_DISTRIBUTION_FREQUENCY * 5 ) == 0 ) //check for randomizer "hit"
				{
					//calculate borders for kernel
					unsigned int yBorder = startY + cycle - 1;
					if( yBorder >= distributionMap.size() )
					{
						yBorder = distributionMap.size() - 1;
					}
					unsigned int xBorder = startX + cycle - 1;
					if( xBorder >= distributionMap[0].size() )
					{
						xBorder = distributionMap.size() - 1;
					}

					//fatten kernel if necessary
					for( unsigned int y = startY; y <= yBorder; y++ )
					{
						for( unsigned int x = startX; x <= xBorder; x++ )
						{
							if( distributionMap[y][x] < PLANTS_DISTRIBUTION_FREQUENCY )
							{
								distributionMap[y][x]++;
							}
						}
					}
				}
			}
		}
	}
}
