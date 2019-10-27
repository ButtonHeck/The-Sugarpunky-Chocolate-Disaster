/*
 * Copyright 2019 Ilya Malgin
 * SettingsManagerImpl.cpp
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
 * Purpose: contains definitions of SettingsManagerImpl class
 */

#include "SettingsManagerImpl"
#include "Logger"

#include <fstream>

/**
* @param ctor that parses settings file
* @param settingsFilepath path to a settings file
*/
SettingsManagerImpl::SettingsManagerImpl( const char * settingsFilepath )
{
	std::ifstream settingsFileStream( settingsFilepath );

	//read all the lines from a file first
	std::vector<std::string> lines;
	std::string line;
	while( std::getline( settingsFileStream, line, '\n' ) )
	{
		lines.push_back( line );
	}
	settingsFileStream.close();

	//process each line one by one
	settingsCategoryName name;
	for( const std::string & line : lines )
	{
		//check if the line contains name of the settings category (this line starts with '[' and ends with ']')
		if( line.find( '[' ) != std::string::npos )
		{
			name = line.substr( line.find( '[' ) + 1, line.find( ']' ) - 1 );
			//check first if the settings map contains storage for a name
			if( settings.find( name ) == settings.end() )
			{
				settings[name] = settingsStorage();
			}
			continue;
		}

		//find positions of the helper symbols
		size_t delimiterIndex = line.find( '=', 0 );
		size_t typeHintIndex = line.find( '<' );
		char typeHint = line[typeHintIndex + 1];

		std::string settingKey = line.substr( 0, typeHintIndex );
		std::string valueStr = line.substr( delimiterIndex + 1 );

		//parse aliasing for a value string (if has one)
		if( valueStr.find( '<' ) != std::string::npos )
		{
			size_t openBracketIndex = valueStr.find( '<' );
			size_t closeBracketIndex = valueStr.find( '>' );
			std::string aliasValueStr = valueStr.substr( openBracketIndex + 1, closeBracketIndex - 1 );

			//substitute this string with a value that matches alias key (an alias key should already be in the map!)
			switch( typeHint )
			{
			case 'i':
				valueStr = std::to_string( getInt( name.c_str(), aliasValueStr.c_str() ) );
				break;
			case 'f':
				valueStr = std::to_string( getFloat( name.c_str(), aliasValueStr.c_str() ) );
				break;
			case 'b':
				valueStr = std::to_string( getBool( name.c_str(), aliasValueStr.c_str() ) );
				break;
			default:
				Logger::log( "invalid type hint found for key: %\n", settingKey );
				throw std::exception();
			}
		}

		//define setting value based on a type hint (single character)
		std::any settingValue;
		switch( typeHint )
		{
		case 'i':
			settingValue = std::stoi( valueStr );
			break;
		case 'f':
			settingValue = std::stof( valueStr );
			break;
		case 'b':
			settingValue = valueStr == "true";
			break;
		default:
			Logger::log( "invalid type hint found for key: %\n", settingKey );
			throw std::exception();
		}

		//put setting token to the settings storage
		settingsStorage & map = settings[name];
		map[settingKey] = settingValue;
	}
}

/**
* @brief tries to fetch an int value from the map for given key from a given category
* @param category settings category
* @param settingKey settings key to check
*/
int SettingsManagerImpl::getInt( const char * category,
								 const char * settingKey )
{
	int result;
	try
	{
		result = std::any_cast<int>( settings[category][settingKey] );
	}
	catch( std::bad_any_cast )
	{
		Logger::log( "unable to cast value for key '%' to 'int' in category %\n", settingKey, category );
		result = 0;
	}
	return result;
}

/**
* @brief tries to fetch a float value from the map for given key from a given category
* @param category settings category
* @param settingKey settings key to check
*/
float SettingsManagerImpl::getFloat( const char * category,
									 const char * settingKey )
{
	float result;
	try
	{
		result = std::any_cast<float>( settings[category][settingKey] );
	}
	catch( std::bad_any_cast )
	{
		Logger::log( "unable to cast value for key '%' to 'float' in category %\n", settingKey, category );
		result = 0.0f;
	}
	return result;
}

/**
* @brief tries to fetch a bool value from the map for given key from a given category
* @param category settings category
* @param settingKey settings key to check
*/
bool SettingsManagerImpl::getBool( const char * category,
								   const char * settingKey )
{
	bool result;
	try
	{
		result = std::any_cast<bool>( settings[category][settingKey] );
	}
	catch( std::bad_any_cast )
	{
		Logger::log( "unable to cast value for key '%' to 'bool' in category %\n", settingKey, category );
		result = false;
	}
	return result;
}
