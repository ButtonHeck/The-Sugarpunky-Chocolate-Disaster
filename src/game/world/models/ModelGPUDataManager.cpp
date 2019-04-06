#include "game/world/models/ModelGPUDataManager.h"

ModelGPUDataManager::ModelGPUDataManager(bool isParentModelLowPoly)
  :
    isLowPoly(isParentModelLowPoly),
    basicGLBuffers(VAO | VBO | INSTANCE_VBO | EBO)
{}

void ModelGPUDataManager::setupBuffers(const std::vector<Mesh::Vertex> &vertices, const std::vector<GLuint> &indices)
{
  indicesSize = indices.size();
  basicGLBuffers.bind(VAO | VBO | EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Mesh::Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indicesSize, &indices[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, Normal));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, TexCoords));
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, Tangent));
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, Bitangent));
  glEnableVertexAttribArray(9);
  //intentionally set GL_FLOAT although the data is a pair of unsigned integers
  glVertexAttribPointer(9, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, TexIndices));

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

  BufferCollection::bindZero(VAO | VBO | EBO);
}

void ModelGPUDataManager::prepareIndirectBufferData(const std::vector<std::pair<ModelChunk, unsigned int>> &visibleChunks,
                                                    unsigned int modelIndex,
                                                    float loadingDistance,
                                                    float loadingDistanceShadow)
{
  BENCHMARK("(ST)Model: prepare indirect buffer", true);
  indirectTokens.clear();
  indirectTokens.reserve(visibleChunks.size());
  indirectTokensShadow.clear();
  indirectTokensShadow.reserve(visibleChunks.size());
  for (unsigned int chunkIndex = 0; chunkIndex < visibleChunks.size(); chunkIndex++)
    {
      const ModelChunk& chunk = visibleChunks[chunkIndex].first;
      unsigned int distanceToChunk = visibleChunks[chunkIndex].second;

      if ((!isLowPoly && distanceToChunk < loadingDistance) || (isLowPoly && distanceToChunk >= loadingDistance))
        {
          unsigned int numInstancesInChunk = chunk.getNumInstances(modelIndex);
          unsigned int instanceOffsetInChunk = chunk.getInstanceOffset(modelIndex);
          addIndirectBufferData(distanceToChunk, indicesSize, numInstancesInChunk, instanceOffsetInChunk, false);
          if (distanceToChunk < loadingDistanceShadow)
            addIndirectBufferData(distanceToChunk, indicesSize, numInstancesInChunk, instanceOffsetInChunk, true);
        }
    }

  GLuint dataOffset = 0;
  for (const auto& token : indirectTokens)
    {
        multiDrawIndirectData[dataOffset++] = token.second.indicesCount;
        multiDrawIndirectData[dataOffset++] = token.second.numInstances;
        multiDrawIndirectData[dataOffset++] = token.second.FIRST_INDEX;
        multiDrawIndirectData[dataOffset++] = token.second.BASE_VERTEX;
        multiDrawIndirectData[dataOffset++] = token.second.instanceOffset;
    }
  drawIndirectCommandPrimCount = indirectTokens.size();

  dataOffset = 0;
  for (const auto& token : indirectTokensShadow)
    {
        multiDrawIndirectDataShadow[dataOffset++] = token.second.indicesCount;
        multiDrawIndirectDataShadow[dataOffset++] = token.second.numInstances;
        multiDrawIndirectDataShadow[dataOffset++] = token.second.FIRST_INDEX;
        multiDrawIndirectDataShadow[dataOffset++] = token.second.BASE_VERTEX;
        multiDrawIndirectDataShadow[dataOffset++] = token.second.instanceOffset;
    }
  drawIndirectCommandPrimCountShadow = indirectTokensShadow.size();
}

void ModelGPUDataManager::updateIndirectBufferData()
{
  BENCHMARK("Model: load indirect data to GPU", true);
  basicGLBuffers.bind(VAO | DIBO);
  glBufferSubData(GL_DRAW_INDIRECT_BUFFER, 0, sizeof(GLuint) * INDIRECT_DRAW_COMMAND_ARGUMENTS * drawIndirectCommandPrimCount, multiDrawIndirectData);
  shadowDIBO.bind(DIBO);
  glBufferSubData(GL_DRAW_INDIRECT_BUFFER, 0, sizeof(GLuint) * INDIRECT_DRAW_COMMAND_ARGUMENTS * drawIndirectCommandPrimCountShadow, multiDrawIndirectDataShadow);
}

void ModelGPUDataManager::loadModelInstances(const std::vector<glm::mat4> &instanceMatrices, unsigned int numInstances)
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
  BufferCollection::bindZero(VAO | VBO);
}

void ModelGPUDataManager::addIndirectBufferData(int distanceToChunk, GLuint indicesSize, GLuint numInstances, GLuint instanceOffset, bool isShadow)
{
  if (!isShadow)
    indirectTokens.emplace_back(distanceToChunk, IndirectBufferToken(indicesSize, numInstances, instanceOffset));
  else
    indirectTokensShadow.emplace_back(distanceToChunk, IndirectBufferToken(indicesSize, numInstances, instanceOffset));
}

GLsizei ModelGPUDataManager::getPrimitiveCount(bool isShadow) const
{
  return isShadow ? drawIndirectCommandPrimCountShadow : drawIndirectCommandPrimCount;
}

BufferCollection &ModelGPUDataManager::getBasicGLBuffers()
{
  return basicGLBuffers;
}

BufferCollection &ModelGPUDataManager::getShadowDIBO()
{
  return shadowDIBO;
}

ModelGPUDataManager::IndirectBufferToken::IndirectBufferToken(GLuint indicesCount, GLuint numInstances, GLuint instanceOffset)
  :
    indicesCount(indicesCount),
    numInstances(numInstances),
    instanceOffset(instanceOffset)
{}
