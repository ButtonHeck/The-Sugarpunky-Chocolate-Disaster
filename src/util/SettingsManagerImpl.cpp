#include "SettingsManagerImpl"
#include "Logger"

#include <fstream>

SettingsManagerImpl::SettingsManagerImpl( const char * settingsFilepath )
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
