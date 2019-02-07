#include "game/world/models/Model.h"

TextureLoader* Model::textureLoader;
void Model::bindTextureLoader(TextureLoader &textureLoader)
{
  Model::textureLoader = &textureLoader;
}

Model::Model(const std::string& path, bool isLowPoly, unsigned int numRepetitions)
  :
    isLowPoly(isLowPoly),
    numRepetitions(numRepetitions),
    GPUDataManager(isLowPoly),
    renderer(GPUDataManager.getBasicGLBuffers(), GPUDataManager.getShadowDIBO())
{
  loadModel(MODELS_DIR + path);
  setup();
}

void Model::loadModel(const std::string &path)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_FlipUVs | aiProcess_Triangulate);
  if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    Logger::log("Error while loading Assimp: %\n", importer.GetErrorString());
  directory = path.substr(0, path.find_last_of('/'));
  GLuint meshVertexIndexOffset = 0;
  processNode(scene->mRootNode, scene, meshVertexIndexOffset);
}

void Model::processNode(aiNode *node, const aiScene* scene, GLuint& meshVertexIndexOffset)
{
  for (unsigned int meshIndex = 0; meshIndex < node->mNumMeshes; meshIndex++)
    {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[meshIndex]];
      Mesh processedMesh = processMesh(mesh, scene, meshVertexIndexOffset);
      GLuint currentMeshNumVertices = processedMesh.vertices.size();
      meshes.emplace_back(std::move(processedMesh));
      meshVertexIndexOffset += currentMeshNumVertices;
    }
  for (unsigned int childNodeIndex = 0; childNodeIndex < node->mNumChildren; childNodeIndex++)
    processNode(node->mChildren[childNodeIndex], scene, meshVertexIndexOffset);
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene* scene, GLuint meshVertexIndexOffset)
{
  static unsigned int diffuseSamplerIndex = 0, specularSamplerIndex = 0;
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;

  for (unsigned int vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++)
    {
      Vertex vertex;
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

      vertices.emplace_back(std::move(vertex));
    }

  //process indices
  for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
    {
      aiFace face = mesh->mFaces[faceIndex];
      for (unsigned int eboIndex = 0; eboIndex < face.mNumIndices; eboIndex++)
        indices.emplace_back(std::move(face.mIndices[eboIndex] + meshVertexIndexOffset));
    }

  //process materials
  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
  loadMaterialTextures(material, aiTextureType_DIFFUSE, "u_texture_diffuse", diffuseSamplerIndex);
  loadMaterialTextures(material, aiTextureType_SPECULAR, "u_texture_specular", specularSamplerIndex);

  return Mesh(vertices, indices);
}

void Model::loadMaterialTextures(aiMaterial *material,
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

void Model::setup()
{
  for (Mesh& mesh : meshes)
    {
      vertices.insert(vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
      indices.insert(indices.end(), mesh.indices.begin(), mesh.indices.end());
    }
  GPUDataManager.setupBuffers(vertices, indices);
}

void Model::draw(bool isShadow)
{
  GLsizei primitiveCount = GPUDataManager.getPrimitiveCount(isShadow);
  renderer.render(isShadow, primitiveCount);
}

void Model::drawOneInstance()
{
  renderer.renderOneInstance(indices.size());
}

void Model::prepareIndirectBufferData(const std::vector<ModelChunk>& chunks,
                                      unsigned int modelIndex,
                                      const glm::vec2& cameraPositionXZ,
                                      const Frustum &frustum,
                                      float loadingDistance,
                                      float loadingDistanceShadow,
                                      float loadingDistanceLowPoly)
{
  GPUDataManager.prepareIndirectBufferData(chunks,
                                           modelIndex,
                                           cameraPositionXZ,
                                           frustum,
                                           loadingDistance,
                                           loadingDistanceShadow,
                                           loadingDistanceLowPoly);
}

void Model::updateIndirectBufferData()
{
  GPUDataManager.updateIndirectBufferData();
}

void Model::loadModelInstances(const std::vector<glm::mat4> &instanceMatrices, unsigned int numInstances)
{
  GPUDataManager.loadModelInstances(instanceMatrices, numInstances);
}

unsigned int Model::getRepeatCount() const
{
  return numRepetitions;
}

void Model::cleanup()
{
  GPUDataManager.cleanup();
}
