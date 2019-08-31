/*
 * Copyright 2019 Ilya Malgin
 * SaveLoadManager.h
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
 * Purpose: contains declaration for SaveLoadManager class
 * @version 0.1.0
 */

#pragma once

#include <string>

class Camera;
class Scene;

/**
* @brief manager to file save/load operations.
* Responsible for handling file i/o streams, ordering serialization/deserialization calls of the game modules data
* that should be stored to or loaded from a file
*/
class SaveLoadManager
{
public:
	SaveLoadManager( Scene & scene, 
					 Camera & camera, 
					 Camera & shadowCamera ) noexcept;
	bool saveToFile( const std::string & filename );
	bool loadFromFile( const std::string & filename );

private:
	Scene & scene;
	Camera & camera;
	/** @todo remove this in release version of the game */
	Camera & shadowCamera;
};
