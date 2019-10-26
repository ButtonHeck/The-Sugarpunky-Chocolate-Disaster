#include "SettingsManager"
#include "Logger"

#include <fstream>

std::unordered_map < settingsCategoryName, settingsStorage > SettingsManager::settings;

void SettingsManager::init( const char * settingsFilepath )
{
	std::ifstream settingsFileStream( settingsFilepath );

	std::vector<std::string> lines;
	std::string line;
	while( std::getline( settingsFileStream, line, '\n' ) )
	{
		lines.push_back( line );
	}
	settingsFileStream.close();

	settingsCategoryName name;
	for( auto & line : lines )
	{
		if( line.find( '[' ) != std::string::npos )
		{
			name = line.substr( line.find( '[' ) + 1, line.find( ']' ) - 1 );
			continue;
		}
		size_t delimiterIndex = line.find( '=', 0 );
		size_t typeHintIndex = line.find( '<' );
		char typeHint = line[typeHintIndex + 1];

		std::string settingKey = line.substr( 0, typeHintIndex );
		std::string valueStr = line.substr( delimiterIndex + 1 );

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

		if( settings.find( name ) == settings.end() )
		{
			settings[name] = settingsStorage();
		}
		settingsStorage & map = settings[name];
		map[settingKey] = settingValue;
	}
}

int SettingsManager::getInt( const std::string & category, 
							 const std::string & settingKey )
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

float SettingsManager::getFloat( const std::string & category, 
								 const std::string & settingKey )
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

bool SettingsManager::getBool( const std::string & category, 
							   const std::string & settingKey )
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

std::string SettingsManager::getStdString( const std::string & category, 
										   const std::string & settingKey )
{
	std::string result;
	try
	{
		result = std::any_cast<std::string>( settings[category][settingKey] );
	}
	catch( std::bad_any_cast )
	{
		Logger::log( "unable to cast value for key '%' to 'std::string' in category %\n", settingKey, category );
	}
	return result;
}
