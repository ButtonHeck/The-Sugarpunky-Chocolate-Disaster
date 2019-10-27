#include "SettingsManager"
#include "SettingsManagerImpl"

std::unique_ptr<SettingsManagerImpl> SettingsManager::impl;

void SettingsManager::init( const char * settingsFilepath )
{
	impl = std::make_unique<SettingsManagerImpl>( settingsFilepath );
}

int SettingsManager::getInt( const char * category,
							 const char * settingKey )
{
	return impl->getInt( category, settingKey );
}

float SettingsManager::getFloat( const char * category,
								 const char * settingKey )
{
	return impl->getFloat( category, settingKey );
}

bool SettingsManager::getBool( const char * category,
							   const char * settingKey )
{
	return impl->getBool( category, settingKey );
}
