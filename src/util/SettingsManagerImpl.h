/*
 * Copyright 2019 Ilya Malgin
 * SettingsManagerImpl.h
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
 * Purpose: contains declarations of SettingsManagerImpl class and some helper typedefs
 */

#pragma once

#include <unordered_map>
#include <any>
#include <string>

typedef std::unordered_map<std::string, std::any> settingsStorage;
typedef std::string settingsCategoryName;

/**
* @brief Implementation of a settings manager mechanism.
* Responsible for parsing data from settings file and managing local settings storage
*/
class SettingsManagerImpl
{
public:
	SettingsManagerImpl( const char * settingsFilepath );
	int getInt( const char * category,
				const char * settingKey );
	float getFloat( const char * category,
					const char * settingKey );
	bool getBool( const char * category,
				  const char * settingKey );

private:
	std::unordered_map < settingsCategoryName, settingsStorage > settings;
};
