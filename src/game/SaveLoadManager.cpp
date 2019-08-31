/*
 * Copyright 2019 Ilya Malgin
 * SaveLoadManager.cpp
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
 * Purpose: contains definitions for SaveLoadManager class
 * @version 0.1.0
 */

#include "SaveLoadManager"
#include "Camera"
#include "Scene"
#include "Logger"

/**
* @brief plain ctor
* @param scene game scene
* @param camera player's camera
* @param shadowCamera auxiliary shadow regions defining camera
*/
SaveLoadManager::SaveLoadManager( Scene & scene, 
								  Camera & camera, 
								  Camera & shadowCamera ) noexcept
	: scene( scene )
	, camera( camera )
	, shadowCamera( shadowCamera )
{}

/**
* @brief handles file saving routine
* @param filename string file name to write data to
*/
bool SaveLoadManager::saveToFile( const std::string & filename )
{
	std::ofstream output( filename );
	if( !output )
	{
		Logger::log( "Could not open file for saving: %\n", filename.c_str() );
		return false;
	}
	scene.serialize( output );
	camera.serialize( output );
	output.close();
	return true;
}

/**
* @brief handles file loading routine
* @param filename string file name to read data from
*/
bool SaveLoadManager::loadFromFile( const std::string & filename )
{
	std::ifstream input( filename );
	if( !input )
	{
		Logger::log( "Could not open file for loading: %\n", filename.c_str() );
		return false;
	}
	scene.deserialize( input );
	camera.deserialize( input );
	shadowCamera = camera; //temporary assignment as long as shadowCamera exists in application code
	input.close();
	return true;
}
