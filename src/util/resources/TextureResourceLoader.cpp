#include "TextureResourceLoader"

#include <fstream>
#include <string>

std::unordered_map<std::string, TextureResource> TextureResourceLoader::textures;

void TextureResourceLoader::initialize( const char * path, 
										const char * flags )
{
	errno_t err;
	FILE * file;
	err = fopen_s( &file, path, flags );
	std::ifstream fileStream( path, std::ios::binary );
	int numTexturesInFile = getc( file );
	textures.reserve( numTexturesInFile );

	for( int textureIndex = 0; textureIndex < numTexturesInFile; textureIndex++ )
	{
		TextureResource textureResource;

		int textureNameLength = getc( file );
		std::unique_ptr<char[]> nameBuffer( new char[textureNameLength + 1] );
		for( size_t charIndex = 0; charIndex < textureNameLength; ++charIndex )
		{
			nameBuffer[charIndex] = getc( file );
		}
		nameBuffer[textureNameLength] = '\0';
		std::string textureName( nameBuffer.get() );
		textureResource.localName = textureName;

		//deserialize texture width
		int texWidthStringLength = getc( file );
		std::unique_ptr<char[]> texWidthStringBuffer( new char[texWidthStringLength + 2] );
		fgets( texWidthStringBuffer.get(), texWidthStringLength + 1, file );
		texWidthStringBuffer[texWidthStringLength + 1] = '\0';
		std::string texWidthString( texWidthStringBuffer.get() );
		int texWidth = std::stoi( texWidthString );
		textureResource.width = texWidth;

		//deserialize texture height
		int texHeightStringLength = getc( file );
		std::unique_ptr<char[]> texHeightStringBuffer( new char[texHeightStringLength + 2] );
		fgets( texHeightStringBuffer.get(), texHeightStringLength + 1, file );
		texHeightStringBuffer[texHeightStringLength + 1] = '\0';
		std::string texHeightString( texHeightStringBuffer.get() );
		int texHeight = std::stoi( texHeightString );
		textureResource.height = texHeight;

		//deserialize texture channels
		int texChannels = getc( file );
		textureResource.channels = texChannels;

		//deserialize texture data size
		int texDataSizeStringLength = getc( file );
		std::unique_ptr<char[]> texDataSizeStringBuffer( new char[texDataSizeStringLength + 2] );
		fgets( texDataSizeStringBuffer.get(), texDataSizeStringLength + 1, file );
		texDataSizeStringBuffer[texDataSizeStringLength + 1] = '\0';
		std::string texDataSizeString( texDataSizeStringBuffer.get() );
		long texDataSize = std::stol( texDataSizeString );
		textureResource.dataSize = texDataSize;

		//deserialize texture data
		textureResource.data = new char[textureResource.dataSize];		
		fpos_t currentPos;
		fgetpos( file, &currentPos );
		fileStream.seekg( currentPos );
		fileStream.read( textureResource.data, textureResource.dataSize );
		fpos_t newPosition = currentPos + textureResource.dataSize;
		fsetpos( file, &newPosition );

		textures[textureResource.localName] = textureResource;
	}
	fclose( file );
	fileStream.close();
}

void TextureResourceLoader::release()
{
	for( auto & token : textures )
	{
		delete[] token.second.data;
	}
	textures.clear();
}

const TextureResource & TextureResourceLoader::getTextureResource( const std::string & textureName )
{
	const TextureResource & resource = textures.at( textureName );
	return resource;
}
