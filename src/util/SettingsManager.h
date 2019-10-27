/*
 * Copyright 2019 Ilya Malgin
 * SettingsManager.h
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
 * Purpose: contains declarations of SettingsManager class
 */

#pragma once

#include <memory>

class SettingsManagerImpl;

/**
* @brief Utility class for managing settings from a dedicated file.
* Responsible for creating an implementation object and delegating tasks to it
*/
class SettingsManager
{
public:
	static void init( const char * settingsFilepath );
	static int getInt( const char * category, 
					   const char * settingKey );
	static float getFloat( const char * category,
						   const char * settingKey );
	static bool getBool( const char * category,
						 const char * settingKey );

private:
	static std::unique_ptr<SettingsManagerImpl> impl;
};
