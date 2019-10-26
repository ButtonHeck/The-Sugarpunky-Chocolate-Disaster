#pragma once

#include <unordered_map>
#include <any>

typedef std::unordered_map<std::string, std::any> settingsStorage;
typedef std::string settingsCategoryName;

class SettingsManager
{
public:
	static void init( const char * settingsFilepath );
	static int getInt( const std::string & category, 
					   const std::string & settingKey );
	static float getFloat( const std::string & category,
						   const std::string & settingKey );
	static bool getBool( const std::string & category,
						 const std::string & settingKey );
	static std::string getStdString( const std::string & category,
									 const std::string & settingKey );

private:
	static std::unordered_map < settingsCategoryName, settingsStorage > settings;
};
