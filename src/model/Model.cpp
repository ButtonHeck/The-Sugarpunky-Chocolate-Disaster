#include "model/Model.h"
#include <IL/il.h>

Model::Model(const std::string& path, bool isLowPoly, unsigned int numRepetitions)
  :
    isLowPoly(isLowPoly),
    numRepetitions(numRepetitions),
    basicGLBuffers(VAO | VBO | INSTANCE_VBO | EBO)
{
  loadModel(std::string(MODELS_DIR + path));
  basicGLBuffers.reserveNameForFutureStorage(DIBO);
  shadowDIBO.reserveNameForFutureStorage(DIBO);
  setup();
}

void Model::loadModel(const std::string &path)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_FlipUVs | aiProcess_Triangulate);
  if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    Logger::log("Error while loading Assimp: %\n", importer.GetErrorString());
  directory = path.substr(0, path.find_last_of('/'));
  GLuint meshIndexOffset = 0;
  processNode(scene->mRootNode, scene, meshIndexOffset);
}

void Model::processNode(aiNode *node, const aiScene* scene, GLuint& meshIndexOffset)
{
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      Mesh processedMesh = processMesh(mesh, scene, meshIndexOffset);
      GLuint currentMeshNumVertices = processedMesh.vertices.size();
      meshes.emplace_back(std::move(processedMesh));
      meshIndexOffset += currentMeshNumVertices;
    }
  for (unsigned int i = 0; i < node->mNumChildren; i++)
    processNode(node->mChildren[i], scene, meshIndexOffset);
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene* scene, GLuint meshIndexOffset)
{
  static unsigned int diffuseSamplerIndex = 0, specularSamplerIndex = 0;
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
      Vertex vertex;
      glm::vec3 vector;
      //position
      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].y;
      vector.z = mesh->mVertices[i].z;
      vertex.Position = vector;
      //normals
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.Normal = vector;
      //TexCoords (if any)
      if (mesh->mTextureCoords[0])
        {
          glm::vec2 texVec;
          texVec.x = mesh->mTextureCoords[0][i].x;
          texVec.y = mesh->mTextureCoords[0][i].y;
          vertex.TexCoords = texVec;
        }
      else
        vertex.TexCoords = glm::vec2(0.0, 0.0);
      //tangent
      vector.x = mesh->mTangents[i].x;
      vector.y = mesh->mTangents[i].y;
      vector.z = mesh->mTangents[i].z;
      vertex.Tangent = vector;
      //bitangent
      vector.x = mesh->mBitangents[i].x;
      vector.y = mesh->mBitangents[i].y;
      vector.z = mesh->mBitangents[i].z;
      vertex.Bitangent = vector;
      //texture type indices
      vertex.TexIndices = glm::uvec2(diffuseSamplerIndex, specularSamplerIndex);
      vertices.emplace_back(std::move(vertex));
    }
  //process indices
  for (unsigned int i=0; i < mesh->mNumFaces; i++)
    {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++)
        indices.emplace_back(std::move(face.mIndices[j] + meshIndexOffset));
    }
  //process materials
  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
  loadMaterialTextures(material, aiTextureType_DIFFUSE, "u_texture_diffuse", diffuseSamplerIndex);
  loadMaterialTextures(material, aiTextureType_SPECULAR, "u_texture_specular", specularSamplerIndex);

  return Mesh(vertices, indices);
}

void Model::loadMaterialTextures(aiMaterial *material,
                                 aiTextureType type,
                                 std::string typeName,
                                 unsigned int& samplerIndex)
{
  for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
      aiString texturePath;
      material->GetTexture(type, i, &texturePath);

      std::string path = this->directory + '/' + std::string(texturePath.C_Str());
      bool useAnisotropy = type == aiTextureType_DIFFUSE;
      GLenum magFilter = type == aiTextureType_DIFFUSE ? GL_LINEAR : GL_NEAREST;
      GLenum minFilter = type == aiTextureType_DIFFUSE ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST;
      bool useNoSRGB = type == aiTextureType_SPECULAR;
      GLuint texture = textureLoader->loadTexture(path,
                                                  0,
                                                  GL_REPEAT,
                                                  magFilter,
                                                  minFilter,
                                                  useAnisotropy,
                                                  false,
                                                  true,
                                                  useNoSRGB);
      GLuint64 textureHandle = glGetTextureHandleARB(texture);
      std::string uniformSamplerString = typeName + "[" + std::to_string(samplerIndex) + "]";
      BindlessTextureManager::emplace_back(uniformSamplerString, texture, textureHandle);
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
  basicGLBuffers.bind(VAO | VBO | EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
  glEnableVertexAttribArray(9);
  //intentionally set GL_FLOAT
  glVertexAttribPointer(9, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexIndices));

  if (basicGLBuffers.get(DIBO) == 0)
    {
      basicGLBuffers.add(DIBO);
      basicGLBuffers.bind(DIBO);
      glNamedBufferStorage(basicGLBuffers.get(DIBO), sizeof(multiDrawIndirectData), 0, GL_DYNAMIC_STORAGE_BIT);
    }
  if (shadowDIBO.get(DIBO) == 0)
    {
      shadowDIBO.add(DIBO);
      shadowDIBO.bind(DIBO);
      glNamedBufferStorage(shadowDIBO.get(DIBO), sizeof(multiDrawIndirectDataShadow), 0, GL_DYNAMIC_STORAGE_BIT);
    }

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

TextureLoader* Model::textureLoader;
void Model::bindTextureLoader(TextureLoader &textureLoader)
{
  Model::textureLoader = &textureLoader;
}

void Model::draw(bool isShadow)
{
  basicGLBuffers.bind(VAO);
  if (!isShadow)
    {
      BENCHMARK("Model: draw", true);
      basicGLBuffers.bind(DIBO);
      glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, drawIndirectCommandPrimCount, 0);
    }
  else
    {
      BENCHMARK("Model: draw shadows", true);
      shadowDIBO.bind(DIBO);
      glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, drawIndirectCommandPrimCountShadow, 0);
    }
}

void Model::prepareMeshesIndirectData(std::vector<ModelChunk>& chunks,
                                      unsigned int index,
                                      const glm::vec2& cameraPositionXZ,
                                      const Frustum &frustum,
                                      float loadingDistance,
                                      float loadingDistanceShadow,
                                      float loadingDistanceLowPoly)
{
  BENCHMARK("(ST)Model: update DIB data", true);
  drawIndirectCommandPrimCount = drawIndirectCommandPrimCountShadow = 0;
  indirectTokensSorted.clear();
  indirectTokensSortedShadow.clear();
  GLuint indicesSize = indices.size();
  for (unsigned int i = 0; i < chunks.size(); i++)
    {
      //if a chunk is farther than the load distance - just discard it
      glm::vec2 directionToChunk = chunks[i].getMidPoint() - cameraPositionXZ;
      unsigned int directionToChunkLength = glm::length2(directionToChunk);
      if (!isLowPoly)
        {
          if (directionToChunkLength < loadingDistance)
            {
              glm::vec2 chunkMidPoint = chunks[i].getMidPoint();
              if (frustum.isInsideXZ(chunkMidPoint.x - HALF_CHUNK_SIZE, chunkMidPoint.y + HALF_CHUNK_SIZE, MODELS_FC_RADIUS) ||
                  frustum.isInsideXZ(chunkMidPoint.x + HALF_CHUNK_SIZE, chunkMidPoint.y + HALF_CHUNK_SIZE, MODELS_FC_RADIUS) ||
                  frustum.isInsideXZ(chunkMidPoint.x + HALF_CHUNK_SIZE, chunkMidPoint.y - HALF_CHUNK_SIZE, MODELS_FC_RADIUS) ||
                  frustum.isInsideXZ(chunkMidPoint.x - HALF_CHUNK_SIZE, chunkMidPoint.y - HALF_CHUNK_SIZE, MODELS_FC_RADIUS))
                {
                  addIndirectBufferData(directionToChunkLength,
                                        indicesSize,
                                        chunks[i].getNumInstances(index),
                                        chunks[i].getInstanceOffset(index),
                                        false);
                  if (directionToChunkLength < loadingDistanceShadow)
                    addIndirectBufferData(directionToChunkLength,
                                          indicesSize,
                                          chunks[i].getNumInstances(index),
                                          chunks[i].getInstanceOffset(index),
                                          true);
                }
            }
        }
      else
        {
          if (directionToChunkLength < loadingDistanceLowPoly && directionToChunkLength >= loadingDistance)
            {
              glm::vec2 chunkMidPoint = chunks[i].getMidPoint();
              if (frustum.isInsideXZ(chunkMidPoint.x - HALF_CHUNK_SIZE, chunkMidPoint.y + HALF_CHUNK_SIZE, MODELS_FC_RADIUS) ||
                  frustum.isInsideXZ(chunkMidPoint.x + HALF_CHUNK_SIZE, chunkMidPoint.y + HALF_CHUNK_SIZE, MODELS_FC_RADIUS) ||
                  frustum.isInsideXZ(chunkMidPoint.x + HALF_CHUNK_SIZE, chunkMidPoint.y - HALF_CHUNK_SIZE, MODELS_FC_RADIUS) ||
                  frustum.isInsideXZ(chunkMidPoint.x - HALF_CHUNK_SIZE, chunkMidPoint.y - HALF_CHUNK_SIZE, MODELS_FC_RADIUS))
                {
                  addIndirectBufferData(directionToChunkLength,
                                        indicesSize,
                                        chunks[i].getNumInstances(index),
                                        chunks[i].getInstanceOffset(index),
                                        false);
                  if (directionToChunkLength < loadingDistanceShadow)
                    addIndirectBufferData(directionToChunkLength,
                                          indicesSize,
                                          chunks[i].getNumInstances(index),
                                          chunks[i].getInstanceOffset(index),
                                          true);
                }
            }
        }
    }
  GLuint dataOffset = 0;
  for (auto& token : indirectTokensSorted)
    {
        multiDrawIndirectData[dataOffset++] = token.second.indicesCount;
        multiDrawIndirectData[dataOffset++] = token.second.numInstances;
        multiDrawIndirectData[dataOffset++] = token.second.FIRST_INDEX;
        multiDrawIndirectData[dataOffset++] = token.second.BASE_VERTEX;
        multiDrawIndirectData[dataOffset++] = token.second.instanceOffset;
    }
  dataOffset = 0;
  for (auto& token : indirectTokensSortedShadow)
    {
        multiDrawIndirectDataShadow[dataOffset++] = token.second.indicesCount;
        multiDrawIndirectDataShadow[dataOffset++] = token.second.numInstances;
        multiDrawIndirectDataShadow[dataOffset++] = token.second.FIRST_INDEX;
        multiDrawIndirectDataShadow[dataOffset++] = token.second.BASE_VERTEX;
        multiDrawIndirectDataShadow[dataOffset++] = token.second.instanceOffset;
    }
}

void Model::updateIndirectBufferData()
{
  {
    BENCHMARK("Model: bind+buffer indirect data", true);
    basicGLBuffers.bind(VAO | DIBO);
    glBufferSubData(GL_DRAW_INDIRECT_BUFFER, 0, sizeof(GLuint) * 5 * drawIndirectCommandPrimCount, multiDrawIndirectData);
    shadowDIBO.bind(DIBO);
    glBufferSubData(GL_DRAW_INDIRECT_BUFFER, 0, sizeof(GLuint) * 5 * drawIndirectCommandPrimCountShadow, multiDrawIndirectDataShadow);
  }
}

void Model::addIndirectBufferData(int directionToChunkLength,
                                  GLuint indicesSize,
                                  GLuint numInstances,
                                  GLuint instanceOffset,
                                  bool isShadow)
{
  if (!isShadow)
    {
      indirectTokensSorted.insert(std::pair<int,IndirectBufferToken>(directionToChunkLength, IndirectBufferToken(indicesSize, numInstances, instanceOffset)));
      ++drawIndirectCommandPrimCount;
    }
  else
    {
      indirectTokensSortedShadow.insert(std::pair<int,IndirectBufferToken>(directionToChunkLength, IndirectBufferToken(indicesSize, numInstances, instanceOffset)));
      ++drawIndirectCommandPrimCountShadow;
    }
}

void Model::loadModelInstances(glm::mat4 *instanceMatrices, unsigned int numInstances)
{
  basicGLBuffers.bind(VAO);
  if (basicGLBuffers.get(INSTANCE_VBO) != 0)
    {
      basicGLBuffers.bind(INSTANCE_VBO);
      glInvalidateBufferData(basicGLBuffers.get(INSTANCE_VBO));
      basicGLBuffers.deleteBuffer(INSTANCE_VBO);
    }
  basicGLBuffers.add(INSTANCE_VBO);
  basicGLBuffers.bind(INSTANCE_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * numInstances, &instanceMatrices[0], GL_STATIC_DRAW);
  for (unsigned int i = 0; i < 4; ++i)
    {
      glEnableVertexAttribArray(i+5);
      glVertexAttribPointer(i+5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
      glVertexAttribDivisor(i+5, 1);
    }
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int Model::getRepeatCount() const
{
  return numRepetitions;
}

Model::IndirectBufferToken::IndirectBufferToken(GLuint indicesCount, GLuint numInstances, GLuint instanceOffset)
  :
    indicesCount(indicesCount),
    numInstances(numInstances),
    instanceOffset(instanceOffset)
{}

void Model::cleanup()
{
  basicGLBuffers.deleteBuffers();
  shadowDIBO.deleteBuffers();
}
