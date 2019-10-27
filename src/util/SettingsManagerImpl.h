#pragma once

#include <unordered_map>
#include <any>

typedef std::unordered_map<std::string, std::any> settingsStorage;
typedef std::string settingsCategoryName;

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
