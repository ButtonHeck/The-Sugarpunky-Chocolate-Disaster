/*
 * Copyright 2019 Ilya Malgin
 * Mesh.cpp
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
 * Purpose: contains definitions for Mesh class
 * @version 0.1.0
 */

#include "Mesh"

/**
 * @brief static function for mesh creation
 * @param mesh Assimp library mesh representation
 * @param diffuseSamplerIndex index for diffuse texture
 * @param specularSamplerIndex index for specular texture
 * @param indexOffset offset for each index of the mesh' vertices
 * @return parsed mesh of a model with proper index offset and texture arrays indices
 */
Mesh Mesh::generate(const aiMesh *mesh, GLuint &diffuseSamplerIndex, GLuint &specularSamplerIndex, GLuint indexOffset)
{
  Mesh resultMesh;
  resultMesh.vertices.reserve(mesh->mNumVertices);

  for (unsigned int vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++)
    {
      Mesh::Vertex vertex;
      glm::vec3 attributeVector;

      //position
      attributeVector.x = mesh->mVertices[vertexIndex].x;
      attributeVector.y = mesh->mVertices[vertexIndex].y;
      attributeVector.z = mesh->mVertices[vertexIndex].z;
      vertex.Position = attributeVector;

      //normals
      attributeVector.x = mesh->mNormals[vertexIndex].x;
      attributeVector.y = mesh->mNormals[vertexIndex].y;
      attributeVector.z = mesh->mNormals[vertexIndex].z;
      vertex.Normal = attributeVector;

      //Texture coordinates (if any)
      if (mesh->mTextureCoords[0])
        {
          glm::vec2 textureCoordinates;
          textureCoordinates.x = mesh->mTextureCoords[0][vertexIndex].x;
          textureCoordinates.y = mesh->mTextureCoords[0][vertexIndex].y;
          vertex.TexCoords = textureCoordinates;
        }
      else
        vertex.TexCoords = glm::vec2(0.0f);

      //tangent (not used yet)
      attributeVector.x = mesh->mTangents[vertexIndex].x;
      attributeVector.y = mesh->mTangents[vertexIndex].y;
      attributeVector.z = mesh->mTangents[vertexIndex].z;
      vertex.Tangent = attributeVector;

      //bitangent (not used yet)
      attributeVector.x = mesh->mBitangents[vertexIndex].x;
      attributeVector.y = mesh->mBitangents[vertexIndex].y;
      attributeVector.z = mesh->mBitangents[vertexIndex].z;
      vertex.Bitangent = attributeVector;

      //texture type indices (used as indices in arrays of bindless texture handlers in a shader)
      vertex.TexIndices = glm::uvec2(diffuseSamplerIndex, specularSamplerIndex);

      resultMesh.vertices.emplace_back(std::move(vertex));
    }

  //process indices, offset applied as there might be other meshes in a model allocated before this one
  for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
    {
      aiFace face = mesh->mFaces[faceIndex];
      for (unsigned int eboIndex = 0; eboIndex < face.mNumIndices; eboIndex++)
        resultMesh.indices.emplace_back(std::move(face.mIndices[eboIndex] + indexOffset));
    }

  return resultMesh;
}

const std::vector<Mesh::Vertex> &Mesh::getVertices() const
{
  return vertices;
}

const std::vector<unsigned int> &Mesh::getIndices() const
{
  return indices;
}
