#include "game/world/models/ModelGPUDataManager.h"

ModelGPUDataManager::ModelGPUDataManager(bool isParentModelLowPoly)
  :
    isLowPoly(isParentModelLowPoly),
    basicGLBuffers(VAO | VBO | INSTANCE_VBO | EBO)
{
  basicGLBuffers.reserveNameForFutureStorage(DIBO);
  shadowDIBO.reserveNameForFutureStorage(DIBO);
}

void ModelGPUDataManager::setupBuffers(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices)
{
  indicesSize = indices.size();
  basicGLBuffers.bind(VAO | VBO | EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indicesSize, &indices[0], GL_STATIC_DRAW);
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
  //intentionally set GL_FLOAT although the data is a pair of unsigned integers
  glVertexAttribPointer(9, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexIndices));
  glEnableVertexAttribArray(10);
  glVertexAttribPointer(10, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, IsLowPoly));

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

void ModelGPUDataManager::prepareIndirectBufferData(const std::vector<ModelChunk> &chunks,
                                                    unsigned int modelIndex,
                                                    const glm::vec2 &cameraPositionXZ,
                                                    const Frustum &frustum,
                                                    float loadingDistance,
                                                    float loadingDistanceShadow,
                                                    float loadingDistanceLowPoly)
{
  BENCHMARK("(ST)Model: prepare indirect buffer", true);
  drawIndirectCommandPrimCount = drawIndirectCommandPrimCountShadow = 0;
  indirectTokens.clear();
  indirectTokensShadow.clear();
  for (unsigned int chunkIndex = 0; chunkIndex < chunks.size(); chunkIndex++)
    {
      glm::vec2 chunkMidPoint = chunks[chunkIndex].getMidPoint();
      glm::vec2 directionToChunkCenter = chunkMidPoint - cameraPositionXZ;
      unsigned int distanceToChunk = glm::length2(directionToChunkCenter);

      //if a chunk is farther than the low-poly render distance - just discard it
      if (distanceToChunk >= loadingDistanceLowPoly)
        continue;

      if ((!isLowPoly && distanceToChunk < loadingDistance) || (isLowPoly && distanceToChunk >= loadingDistance))
        {
          if (chunks[chunkIndex].isInsideFrustum(frustum))
            {
              addIndirectBufferData(distanceToChunk, indicesSize, chunks[chunkIndex].getNumInstances(modelIndex), chunks[chunkIndex].getInstanceOffset(modelIndex), false);
              if (distanceToChunk < loadingDistanceShadow)
                addIndirectBufferData(distanceToChunk, indicesSize, chunks[chunkIndex].getNumInstances(modelIndex), chunks[chunkIndex].getInstanceOffset(modelIndex), true);
            }
        }
    }

  GLuint dataOffset = 0;
  for (auto& token : indirectTokens)
    {
        multiDrawIndirectData[dataOffset++] = token.second.indicesCount;
        multiDrawIndirectData[dataOffset++] = token.second.numInstances;
        multiDrawIndirectData[dataOffset++] = token.second.FIRST_INDEX;
        multiDrawIndirectData[dataOffset++] = token.second.BASE_VERTEX;
        multiDrawIndirectData[dataOffset++] = token.second.instanceOffset;
    }
  dataOffset = 0;
  for (auto& token : indirectTokensShadow)
    {
        multiDrawIndirectDataShadow[dataOffset++] = token.second.indicesCount;
        multiDrawIndirectDataShadow[dataOffset++] = token.second.numInstances;
        multiDrawIndirectDataShadow[dataOffset++] = token.second.FIRST_INDEX;
        multiDrawIndirectDataShadow[dataOffset++] = token.second.BASE_VERTEX;
        multiDrawIndirectDataShadow[dataOffset++] = token.second.instanceOffset;
    }
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
    {
      indirectTokens.insert(std::pair<int,IndirectBufferToken>(distanceToChunk, IndirectBufferToken(indicesSize, numInstances, instanceOffset)));
      ++drawIndirectCommandPrimCount;
    }
  else
    {
      indirectTokensShadow.insert(std::pair<int,IndirectBufferToken>(distanceToChunk, IndirectBufferToken(indicesSize, numInstances, instanceOffset)));
      ++drawIndirectCommandPrimCountShadow;
    }
}

void ModelGPUDataManager::cleanup()
{
  basicGLBuffers.deleteBuffers();
  shadowDIBO.deleteBuffers();
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
