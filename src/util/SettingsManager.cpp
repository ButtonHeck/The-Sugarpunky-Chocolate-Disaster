/*
 * Copyright 2019 Ilya Malgin
 * SettingsManager.cpp
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
 * Purpose: contains definitions of SettingsManager class
 */

#include "SettingsManager"
#include "SettingsManagerImpl"

/**
* @brief pointer to settings manager implementation object
*/
std::unique_ptr<SettingsManagerImpl> SettingsManager::impl;

/**
* @brief initializes implementation object
*/
void SettingsManager::init( const char * settingsFilepath )
{
	impl = std::make_unique<SettingsManagerImpl>( settingsFilepath );
}

/**
* @brief returns int value for given key and for a given settings category from implementation
* @param category settings category
* @param settingKey settings key to check
*/
int SettingsManager::getInt( const char * category,
							 const char * settingKey )
{
	return impl->getInt( category, settingKey );
}

/**
* @brief returns float value for given key and for a given settings category from implementation
* @param category settings category
* @param settingKey settings key to check
*/
float SettingsManager::getFloat( const char * category,
								 const char * settingKey )
{
	return impl->getFloat( category, settingKey );
}

/**
* @brief returns bool value for given key and for a given settings category from implementation
* @param category settings category
* @param settingKey settings key to check
*/
bool SettingsManager::getBool( const char * category,
							   const char * settingKey )
{
	return impl->getBool( category, settingKey );
}
