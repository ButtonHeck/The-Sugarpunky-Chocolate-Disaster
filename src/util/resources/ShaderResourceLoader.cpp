#include "ShaderResourceLoader.h"

#include <string>

std::unordered_map<std::string, ShaderResource> ShaderResourceLoader::shaders;

void ShaderResourceLoader::initialize( const char * path, 
									   const char * flags )
{
	errno_t err;
	FILE * file;
	err = fopen_s( &file, path, flags );
	int numShaderSources = getc( file );
	shaders.reserve( numShaderSources );

	for( int shaderIndex = 0; shaderIndex < numShaderSources; shaderIndex++ )
	{
		ShaderResource resource;

		//deserialize name
		int shaderNameLength = getc( file );
		std::unique_ptr<char[]> nameBuffer( new char[shaderNameLength + 1] );
		fpos_t currentPos;
		fgetpos( file, &currentPos );
		fread( nameBuffer.get(), 1, shaderNameLength, file );
		currentPos += shaderNameLength;
		fsetpos( file, &currentPos );
		nameBuffer[shaderNameLength] = '\0';
		std::string shaderName( nameBuffer.get() );
		resource.localName = shaderName;

		//deserialize type
		resource.type = getc( file );

		//deserialize shader source string length
		int shaderSizeStringLength = getc( file );
		std::unique_ptr<char[]> shaderSizeStringBuffer( new char[shaderSizeStringLength + 2] );
		fgets( shaderSizeStringBuffer.get(), shaderSizeStringLength + 1, file );
		shaderSizeStringBuffer[shaderSizeStringLength + 1] = '\0';
		std::string shaderSizeString( shaderSizeStringBuffer.get() );
		int shaderSourceLength = std::stoi( shaderSizeString );
		resource.dataSize = shaderSourceLength;

		//deserialize shader source text
		resource.data = new char[resource.dataSize + 1];
		fgetpos( file, &currentPos );
		fread( resource.data, 1, resource.dataSize, file );
		currentPos += resource.dataSize;
		fsetpos( file, &currentPos );
		resource.data[resource.dataSize] = '\0';

		shaders[resource.localName] = resource;
	}
	fclose( file );
}

void ShaderResourceLoader::release()
{
	for( auto & token : shaders )
	{
		delete[] token.second.data;
	}
	shaders.clear();
}

const ShaderResource & ShaderResourceLoader::getShaderResource( const std::string & shaderSourceName )
{
	const ShaderResource & resource = shaders.at( shaderSourceName );
	return resource;
}
