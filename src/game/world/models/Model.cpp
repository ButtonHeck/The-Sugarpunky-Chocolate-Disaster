/*
 * Copyright 2019 Ilya Malgin
 * Model.cpp
 * This file is part of The Sugarpunky Chocolate Disaster project
 *
 * The Sugarpunky Chocolate Disaster project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Sugarpunky Chocolate Disaster project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * See <http://www.gnu.org/licenses/>
 *
 * Purpose: contains definition for Model class
 * @version 0.1.0
 */

#include "Model"
#include "BindlessTextureManager"
#include "TextureLoader"
#include "DirectoriesSettings"
#include "Logger"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

TextureLoader* Model::textureLoader;
void Model::bindTextureLoader(TextureLoader &textureLoader) noexcept
{
  Model::textureLoader = &textureLoader;
}

/**
 * @brief plain ctor
 * @param path model's relative path to preset models directory
 * @param isLowPoly defines whether this model would be approached as low-poly
 * @param numRepetitions defines how many times in a row this model would be used during allocation on the map
 * @param isInstanced defines whether this model would be rendered with instancing
 */
Model::Model(const std::string& path, bool isLowPoly, unsigned int numRepetitions, bool isInstanced)
  :
    isInstanced(isInstanced),
    isLowPoly(isLowPoly),
    numRepetitions(numRepetitions),
    GPUDataManager(isLowPoly),
    renderer(GPUDataManager.getBasicGLBuffers(), GPUDataManager.getDepthmapDIBO())
{
  load(MODELS_DIR + path);
}

/**
 * @brief parses assimp data to own data storage
 * @param path model's relative path to preset models directory
 */
void Model::load(const std::string &path)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace | 
												 aiProcess_FlipUVs | 
												 aiProcess_Triangulate | 
												 aiProcess_JoinIdenticalVertices);
  if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    Logger::log("Error while loading Assimp: %\n", importer.GetErrorString());
  else
  {
	  directory = path.substr(0, path.find_last_of('/'));
	  GLuint meshVertexIndexOffset = 0;
	  processNode(scene->mRootNode, scene, meshVertexIndexOffset);
	  GPUDataManager.setupBuffers(vertices, indices, isInstanced);
	  //once we have model's data loaded to GPU memory we don't need it on the CPU side
	  vertices.clear();
	  indices.clear();
  }
}

/**
 * @brief parses particular mesh node
 * @param node current node to process
 * @param scene model's scene
 * @param meshVertexIndexOffset offset applied for node vertices in the index buffer
 */
void Model::processNode(const aiNode *node, const aiScene* scene, GLuint& meshVertexIndexOffset)
{
  static unsigned int diffuseSamplerIndex = 0, specularSamplerIndex = 0;
  for (unsigned int meshIndex = 0; meshIndex < node->mNumMeshes; meshIndex++)
    {
      const aiMesh* mesh = scene->mMeshes[node->mMeshes[meshIndex]];
      Mesh processedMesh = Mesh::generate(mesh, diffuseSamplerIndex, specularSamplerIndex, meshVertexIndexOffset);
      const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
      loadMaterialTextures(material, aiTextureType_DIFFUSE, "u_textureDiffuse", diffuseSamplerIndex);
      loadMaterialTextures(material, aiTextureType_SPECULAR, "u_textureSpecular", specularSamplerIndex);
      vertices.insert(vertices.end(), processedMesh.getVertices().begin(), processedMesh.getVertices().end());
      indices.insert(indices.end(), processedMesh.getIndices().begin(), processedMesh.getIndices().end());
      meshVertexIndexOffset += processedMesh.getVertices().size();
    }
  for (unsigned int childNodeIndex = 0; childNodeIndex < node->mNumChildren; childNodeIndex++)
    processNode(node->mChildren[childNodeIndex], scene, meshVertexIndexOffset);
}

/**
 * @brief load texture from given material and add it to bindless texture manager
 * @param material material to load texture from
 * @param type texture type
 * @param uniformName glsl uniform name for the texture
 * @param samplerIndex index for glsl sampler array in a shader
 */
void Model::loadMaterialTextures(const aiMaterial *material,
                                 aiTextureType type,
                                 const std::string& uniformName,
                                 unsigned int& samplerIndex)
{
  for (unsigned int textureIndex = 0; textureIndex < material->GetTextureCount(type); textureIndex++)
    {
      aiString texturePath;
      material->GetTexture(type, textureIndex, &texturePath);
      std::string path = this->directory + '/' + texturePath.C_Str();
      GLenum magFilter = type == aiTextureType_DIFFUSE ? GL_LINEAR : GL_NEAREST;
      GLenum minFilter = type == aiTextureType_DIFFUSE ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST;
      bool useNoSRGB = type == aiTextureType_SPECULAR;
      GLuint texture = textureLoader->loadTexture(path, 0, GL_REPEAT, magFilter, minFilter, true, false, true, useNoSRGB);
      BindlessTextureManager::emplaceBack(uniformName + "[" + std::to_string(samplerIndex) + "]", texture, BINDLESS_TEXTURE_MODEL);
      samplerIndex++;
    }
}

/**
 * @brief delegates a draw call to renderer
 * @param isShadow define whether depthmap or plain on-screen rendering mode is on
 */
void Model::draw(bool isShadow)
{
  renderer.render(isShadow, GPUDataManager.getPrimitiveCount(isShadow));
}

void Model::drawOneInstance()
{
  renderer.renderOneInstance(GPUDataManager.getIndicesCount());
}

/**
 * @brief delegates indirect buffer data preparation to model's data manager
 * @param visibleChunks model's chunks storage with corresponding distance from the camera position
 * @param modelIndex model's index in a chunks storage
 * @param loadingDistance rendering distance for full-res models
 * @param loadingDistanceShadow rendering distance for models' shadows
 */
void Model::prepareIndirectBufferData(const std::vector<std::pair<ModelChunk, unsigned int>>& visibleChunks,
                                      unsigned int modelIndex,
                                      float loadingDistance,
                                      float loadingDistanceShadow)
{
  GPUDataManager.prepareIndirectBufferData(visibleChunks, modelIndex, loadingDistance, loadingDistanceShadow);
}

/**
 * @brief delegates indirect buffer data GPU update to model's data manager
 */
void Model::updateIndirectBufferData()
{
  GPUDataManager.updateIndirectBufferData();
}

/**
 * @brief delegates model's instances data loading to data manager
 * @param instanceMatrices instance matrices storage
 */
void Model::loadModelInstances(const std::vector<glm::mat4> &instanceMatrices)
{
  GPUDataManager.loadModelInstancesData(instanceMatrices);
}

unsigned int Model::getRepeatCount() const noexcept
{
  return numRepetitions;
}
