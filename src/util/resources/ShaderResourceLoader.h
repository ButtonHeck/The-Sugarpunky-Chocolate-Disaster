#pragma once

#include <unordered_map>

struct ShaderResource
{
	std::string localName;
	int type;
	size_t dataSize;
	char * data;
};

class ShaderResourceLoader
{
public:
	ShaderResourceLoader() = delete;
	static void initialize( const char * path,
							const char * flags );
	static void release();
	static const ShaderResource & getShaderResource( const std::string & shaderSourceName );

private:
	static std::unordered_map<std::string, ShaderResource> shaders;
};
