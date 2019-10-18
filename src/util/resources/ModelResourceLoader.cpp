#include "ModelResourceLoader.h"
#include "Mesh"

#include <fstream>

std::unordered_map<std::string, ModelResource> ModelResourceLoader::models;

void ModelResourceLoader::initialize( const char * path )
{
	std::ifstream file( path, std::ios::binary );

	//read number of models
	int numModels = 0;
	file.read( reinterpret_cast<char*>( &numModels ), sizeof( numModels ) );
	models.reserve( numModels );

	//read models one by one
	for( int modelIndex = 0; modelIndex < numModels; modelIndex++ )
	{
		deserializeModel( file );
	}

	file.close();
}

void ModelResourceLoader::deserializeModel( std::ifstream & file )
{
	ModelResource modelResource;

	//read name length
	int nameLength = 0;
	file.read( reinterpret_cast<char*>( &nameLength ), sizeof( nameLength ) );

	//read name
	std::unique_ptr<char[]> nameBuffer( new char[nameLength + 1] );
	file.read( nameBuffer.get(), nameLength );
	nameBuffer[nameLength] = '\0';
	std::string name( nameBuffer.get() );
	modelResource.localName = name;

	//read vertices count
	int verticesCount = 0;
	file.read( reinterpret_cast<char*>( &verticesCount ), sizeof( verticesCount ) );
	modelResource.numVertices = verticesCount;
	//read vertices data
	size_t vertexSize = sizeof( Mesh::Vertex );
	int verticesNumBytes = vertexSize * verticesCount;
	modelResource.verticesData = new char[verticesNumBytes];
	file.read( modelResource.verticesData, verticesNumBytes );

	//read indices count
	int indicesCount = 0;
	file.read( reinterpret_cast<char*>( &indicesCount ), sizeof( indicesCount ) );
	modelResource.numIndices = indicesCount;
	//read indices data
	size_t indexSize = sizeof( unsigned int );
	int indicesNumBytes = indexSize * indicesCount;
	modelResource.indicesData = new char[indicesNumBytes];
	file.read( modelResource.indicesData, indicesNumBytes );

	//read diffuse textures count
	int numDiffuseTextures = 0;
	file.read( reinterpret_cast<char*>( &numDiffuseTextures ), sizeof( numDiffuseTextures ) );
	modelResource.numDiffuseTextures = numDiffuseTextures;
	modelResource.diffuseTextures.reserve( numDiffuseTextures );
	//read diffuse textures
	for( int textureIndex = 0; textureIndex < numDiffuseTextures; textureIndex++ )
	{
		ModelResourceTextureData textureData;

		//read sampler index
		int samplerIndex = 0;
		file.read( reinterpret_cast<char*>( &samplerIndex ), sizeof( samplerIndex ) );
		textureData.samplerIndex = samplerIndex;

		//read texture name length
		int textureNameLength = 0;
		file.read( reinterpret_cast<char*>( &textureNameLength ), sizeof( textureNameLength ) );

		//read texture name
		std::unique_ptr<char[]> textureNameBuffer( new char[textureNameLength + 1] );
		file.read( textureNameBuffer.get(), textureNameLength );
		textureNameBuffer[textureNameLength] = '\0';
		std::string textureName( textureNameBuffer.get() );
		textureData.localName = textureName;
		modelResource.diffuseTextures.push_back( textureData );
	}

	//read specular textures count
	int numSpecularTextures = 0;
	file.read( reinterpret_cast<char*>( &numSpecularTextures ), sizeof( numSpecularTextures ) );
	modelResource.numSpecularTextures = numSpecularTextures;
	modelResource.specularTextures.reserve( numSpecularTextures );
	//read specular textures
	for( int textureIndex = 0; textureIndex < numSpecularTextures; textureIndex++ )
	{
		ModelResourceTextureData textureData;

		//read sampler index
		int samplerIndex = 0;
		file.read( reinterpret_cast<char*>( &samplerIndex ), sizeof( samplerIndex ) );
		textureData.samplerIndex = samplerIndex;

		//read texture name length
		int textureNameLength = 0;
		file.read( reinterpret_cast<char*>( &textureNameLength ), sizeof( textureNameLength ) );

		//read texture name
		std::unique_ptr<char[]> textureNameBuffer( new char[textureNameLength + 1] );
		file.read( textureNameBuffer.get(), textureNameLength );
		textureNameBuffer[textureNameLength] = '\0';
		std::string textureName( textureNameBuffer.get() );
		textureData.localName = textureName;
		modelResource.specularTextures.push_back( textureData );
	}
}

void ModelResourceLoader::release()
{
	for( auto & token : models )
	{
		delete[] token.second.verticesData;
		delete[] token.second.indicesData;
	}
	models.clear();
}

const ModelResource & ModelResourceLoader::getModelResource( const std::string & localName )
{
	const ModelResource & resource = models.at( localName );
	return resource;
}
