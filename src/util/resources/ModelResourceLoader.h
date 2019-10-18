#pragma once

#include <unordered_map>

struct ModelResourceTextureData
{
	std::string localName;
	int samplerIndex;
};

struct ModelResource
{
	std::string localName;
	int numVertices;
	char * verticesData;
	int numIndices;
	char * indicesData;
	int numDiffuseTextures;
	std::vector<ModelResourceTextureData> diffuseTextures;
	int numSpecularTextures;
	std::vector<ModelResourceTextureData> specularTextures;
};

class ModelResourceLoader
{
public:
	ModelResourceLoader() = delete;
	static void initialize( const char * path );
	static void release();
	static const ModelResource & getModelResource( const std::string & localName );

private:
	static void deserializeModel( std::ifstream & file );
	static std::unordered_map<std::string, ModelResource> models;
};
