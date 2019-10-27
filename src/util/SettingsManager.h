#pragma once

#include <memory>

class SettingsManagerImpl;

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
