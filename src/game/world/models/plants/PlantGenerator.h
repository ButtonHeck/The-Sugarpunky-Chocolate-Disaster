/*
 * Copyright 2019 Ilya Malgin
 * PlantGenerator.h
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
 * Purpose: contains declaration for PlantGenerator class
 * @version 0.1.0
 */

#pragma once

#include "ModelChunk"
#include "TypeAliases"
#include "SceneSettings"

#include <vector>
#include <fstream>
#include <memory>
#include <random>

class Model;
class Camera;

/**
 * @brief Boilerplate generator for all the plants.
 * Responsible for defining distances of models' LOD (global), storing models, managing their chunks and their instance matrices,
 * including (de)serialization and indirect buffer updates
 */
class PlantGenerator
{
public:
	PlantGenerator() noexcept;
	void serialize( std::ofstream & output );
	void deserialize( std::ifstream & input );
	void initializeModelRenderChunks( const map2D_f & map,
									  const float approximateHeight );
	void prepareIndirectBufferData( const Camera & camera, 
									const Frustum & viewFrustum,
									const map2D_f & hillMap );
	void updateIndirectBufferData();
	std::vector<Model> & getModels( bool isLowPoly ) noexcept;
	std::vector<ModelChunk> & getChunks() noexcept;
	unsigned int getLoadingDistanceLowPoly() const noexcept;

protected:
	void initializeModelChunks( const map2D_f & map );
	void loadMatrices( const map2D_mat4 & newMatrices );
	map2D_mat4 substituteMatricesStorage();
	bool testHillsOcclusionChunk( const Camera & camera, 
								  const ModelChunk & chunk,
								  const map2D_f & hillMap );
	bool testHillsOcclusionPoint( const glm::vec3 & point,
								  const glm::vec3 & viewPosition,
								  const map2D_f & hillMap );
	float hillOccluderHeightAt( const float interpolantCoord,
								const float fixedCoord,
								const bool fixedCoordIsX,
								const map2D_f & hillMap );

	std::vector<Model> models;
	std::vector<Model> lowPolyModels;
	map2D_mat4 matrices;
	std::unique_ptr<unsigned int[]> numPlants;
	std::vector<ModelChunk> chunks;
	decltype( chunks ) renderChunks;
	float cullingOffset;
	std::default_random_engine randomizer;

private:
    /**
     * @brief LOADING_DISTANCE_CHUNKS define distance of how far would plain models be seen
     */
	const unsigned int LOADING_DISTANCE_CHUNKS;
	const unsigned int LOADING_DISTANCE_UNITS;
	const unsigned int LOADING_DISTANCE_UNITS_SQUARE;
	/**
	 * @brief LOADING_DISTANCE_CHUNKS_LOWPOLY define distance of how far would low-poly models be seen
	 */
	const unsigned int LOADING_DISTANCE_CHUNKS_LOWPOLY;
	const unsigned int LOADING_DISTANCE_UNITS_LOWPOLY;
	const unsigned int LOADING_DISTANCE_UNITS_LOWPOLY_SQUARE;
	/**
	 * @brief LOADING_DISTANCE_CHUNKS_SHADOW define distance of how far would models be casting shadows
	 */
	const unsigned int LOADING_DISTANCE_CHUNKS_SHADOW;
	const unsigned int LOADING_DISTANCE_UNITS_SHADOW;
	const unsigned int LOADING_DISTANCE_UNITS_SHADOW_SQUARE;
};
