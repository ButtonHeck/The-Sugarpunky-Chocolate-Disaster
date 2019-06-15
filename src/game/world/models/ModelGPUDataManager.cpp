#include "ModelGPUDataManager"
#include "BenchmarkTimer"
#include "ModelChunk"

ModelGPUDataManager::ModelGPUDataManager(bool isParentModelLowPoly)
  :
    isLowPoly(isParentModelLowPoly),
    basicGLBuffers(VAO | VBO | INSTANCE_VBO | EBO)
{}

void ModelGPUDataManager::setupBuffers(const std::vector<Mesh::Vertex> &vertices, const std::vector<GLuint> &indices, bool useIndirectBuffer)
{
  indicesCount = indices.size();
  basicGLBuffers.bind(VAO | VBO | EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Mesh::Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indicesCount, &indices[0], GL_STATIC_DRAW);
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

  if (useIndirectBuffer)
    setupIndirectBuffer();

  BufferCollection::bindZero(VAO | VBO | EBO);
}

void ModelGPUDataManager::setupIndirectBuffer()
{
  GLuint numElements = NUM_CHUNKS * INDIRECT_DRAW_COMMAND_ARGUMENTS;
  GLsizei sizeInBytes = sizeof(GLuint) * numElements;
  if (basicGLBuffers.get(DIBO) == 0)
    {
      multiDrawIndirectData = std::make_unique<GLuint[]>(numElements);
      basicGLBuffers.add(DIBO);
      basicGLBuffers.bind(DIBO);
      glNamedBufferStorage(basicGLBuffers.get(DIBO), sizeInBytes, 0, GL_DYNAMIC_STORAGE_BIT);
    }
  if (shadowDIBO.get(DIBO) == 0)
    {
      multiDrawIndirectDataShadow = std::make_unique<GLuint[]>(numElements);
      shadowDIBO.add(DIBO);
      shadowDIBO.bind(DIBO);
      glNamedBufferStorage(shadowDIBO.get(DIBO), sizeInBytes, 0, GL_DYNAMIC_STORAGE_BIT);
    }
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

	  unsigned int numInstancesInChunk = chunk.getNumInstances(modelIndex);
	  //no need to process chunk if this model is not presented in there
	  if (numInstancesInChunk == 0)
		  continue;
	  unsigned int instanceOffsetInChunk = chunk.getInstanceOffset(modelIndex);
	  if (distanceToChunk < loadingDistanceShadow)
	  {
		  if (!isLowPoly)
		  {
			  if (distanceToChunk < loadingDistance)
				//draw nearby camera if fullpoly
				addIndirectBufferData(numInstancesInChunk, instanceOffsetInChunk, false);
		  }
		  else
		  {
			  //draw as shadow source if lowpoly
			  addIndirectBufferData(numInstancesInChunk, instanceOffsetInChunk, true);
		  }
	  }
	  //draw farther from camera if lowpoly
	  if (isLowPoly && distanceToChunk >= loadingDistance)
		  addIndirectBufferData(numInstancesInChunk, instanceOffsetInChunk, false);
    }

  GLuint dataOffset = 0;
  for (const auto& token : indirectTokens)
    {
        multiDrawIndirectData[dataOffset++] = indicesCount;
        multiDrawIndirectData[dataOffset++] = token.numInstances;
        multiDrawIndirectData[dataOffset++] = token.FIRST_INDEX;
        multiDrawIndirectData[dataOffset++] = token.BASE_VERTEX;
        multiDrawIndirectData[dataOffset++] = token.instanceOffset;
    }
  drawIndirectCommandPrimCount = indirectTokens.size();

  dataOffset = 0;
  for (const auto& token : indirectTokensShadow)
    {
        multiDrawIndirectDataShadow[dataOffset++] = indicesCount;
        multiDrawIndirectDataShadow[dataOffset++] = token.numInstances;
        multiDrawIndirectDataShadow[dataOffset++] = token.FIRST_INDEX;
        multiDrawIndirectDataShadow[dataOffset++] = token.BASE_VERTEX;
        multiDrawIndirectDataShadow[dataOffset++] = token.instanceOffset;
    }
  drawIndirectCommandPrimCountShadow = indirectTokensShadow.size();
}

void ModelGPUDataManager::updateIndirectBufferData()
{
  BENCHMARK("Model: load indirect data to GPU", true);
  basicGLBuffers.bind(DIBO);
  glBufferSubData(GL_DRAW_INDIRECT_BUFFER, 0, INDIRECT_DRAW_COMMAND_BYTE_SIZE * drawIndirectCommandPrimCount, multiDrawIndirectData.get());
  shadowDIBO.bind(DIBO);
  glBufferSubData(GL_DRAW_INDIRECT_BUFFER, 0, INDIRECT_DRAW_COMMAND_BYTE_SIZE * drawIndirectCommandPrimCountShadow, multiDrawIndirectDataShadow.get());
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

void ModelGPUDataManager::addIndirectBufferData(GLuint numInstances, GLuint instanceOffset, bool isShadow)
{
  std::vector<IndirectBufferToken>& tokens = isShadow ? indirectTokensShadow : indirectTokens;
  tokens.emplace_back(numInstances, instanceOffset);
}

GLsizei ModelGPUDataManager::getPrimitiveCount(bool isShadow) const noexcept
{
  return isShadow ? drawIndirectCommandPrimCountShadow : drawIndirectCommandPrimCount;
}

BufferCollection &ModelGPUDataManager::getBasicGLBuffers() noexcept
{
  return basicGLBuffers;
}

BufferCollection &ModelGPUDataManager::getShadowDIBO() noexcept
{
  return shadowDIBO;
}

ModelGPUDataManager::IndirectBufferToken::IndirectBufferToken(GLuint numInstances, GLuint instanceOffset) noexcept
  :
    numInstances(numInstances),
    instanceOffset(instanceOffset)
{}
