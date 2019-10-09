#pragma once

#include "DirectoriesSettings"

#include <string>
#include <unordered_map>

struct TextureResource
{
	std::string localName;
	int width;
	int height;
	int channels;
	size_t dataSize;
	unsigned char * data;
};

class TextureResourceLoader
{
public:
	TextureResourceLoader() = delete;
	static void initialize( const char * path,
							const char * flags );
	static void release();
	static const TextureResource & getTextureResource( const std::string & textureName );

private:
	static std::unordered_map<std::string, TextureResource> textures;
};