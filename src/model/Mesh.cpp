#include "model/Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<GLuint> indices)
  :
    vertices(vertices),
    textures(textures),
    indices(indices),
    basicGLBuffers(VAO | VBO | INSTANCE_VBO | EBO)
{
  basicGLBuffers.reserveNameForFutureStorage(DIBO);
}

Mesh::Mesh(Mesh &&old) noexcept
  :
    vertices(old.vertices),
    textures(old.textures),
    indices(old.indices),
    basicGLBuffers(std::move(old.basicGLBuffers)),
    numInstances(old.numInstances),
    indirectTokensSorted(old.indirectTokensSorted),
    drawIndirectCommandPrimCount(old.drawIndirectCommandPrimCount)
{}

Mesh::Mesh(const Mesh &rhs)
  :
    vertices(rhs.vertices),
    textures(rhs.textures),
    indices(rhs.indices),
    basicGLBuffers(rhs.basicGLBuffers),
    numInstances(rhs.numInstances),
    indirectTokensSorted(rhs.indirectTokensSorted),
    drawIndirectCommandPrimCount(rhs.drawIndirectCommandPrimCount)
{}

void Mesh::cleanup()
{
  basicGLBuffers.deleteBuffers();
}

void Mesh::setup()
{
  basicGLBuffers.bind(VAO | VBO | EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
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

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

  if (basicGLBuffers.get(DIBO) == 0)
    {
      basicGLBuffers.add(DIBO);
      basicGLBuffers.bind(DIBO);
      glNamedBufferStorage(basicGLBuffers.get(DIBO), sizeof(multiDrawIndirectData), 0, GL_DYNAMIC_STORAGE_BIT);
    }

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::setupInstances(glm::mat4 *models, unsigned int numModels)
{
  numInstances = numModels;
  basicGLBuffers.bind(VAO);
  if (basicGLBuffers.get(INSTANCE_VBO) != 0)
    {
      basicGLBuffers.bind(INSTANCE_VBO);
      glInvalidateBufferData(basicGLBuffers.get(INSTANCE_VBO));
      basicGLBuffers.deleteBuffer(INSTANCE_VBO);
    }
  basicGLBuffers.add(INSTANCE_VBO);
  basicGLBuffers.bind(INSTANCE_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * numModels, &models[0], GL_STATIC_DRAW);
  for (unsigned int i = 0; i < 4; ++i)
    {
      glEnableVertexAttribArray(i+5);
      glVertexAttribPointer(i+5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
      glVertexAttribDivisor(i+5, 1);
    }
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::draw(bool bindTexture)
{  
  BENCHMARK("Mesh: draw (full func)", true);
  if (bindTexture)
    {
      for(unsigned int i = 0; i < textures.size(); i++)
        {
          glActiveTexture(GL_TEXTURE0 + TEX_MESH_DIFFUSE + i);
          glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
    }
  {
    BENCHMARK("Mesh: multiDrawIndirect", true);
    basicGLBuffers.bind(VAO | DIBO);
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, drawIndirectCommandPrimCount, 0);
  }
}

void Mesh::prepareIndirectBufferData(std::vector<ModelChunk>& chunks,
                                     unsigned int index,
                                     const glm::vec2 &cameraPositionXZ,
                                     const Frustum &frustum)
{
  BENCHMARK("(ST)Mesh: update DIB data", true);
  drawIndirectCommandPrimCount = 0;
  indirectTokensSorted.clear();
  GLuint indicesSize = indices.size();
  for (unsigned int i = 0; i < chunks.size(); i++)
    {
      //if a chunk is farther than the load distance - just discard it
      glm::vec2 directionToChunk = chunks[i].getMidPoint() - cameraPositionXZ;
      unsigned int directionToChunkLength = glm::length2(directionToChunk);
      if (directionToChunkLength > LOADING_DISTANCE_UNITS_SQUARE)
        continue;

      glm::vec2 chunkMidPoint = chunks[i].getMidPoint();
      if (frustum.isInsideXZ(chunkMidPoint.x - HALF_CHUNK_SIZE, chunkMidPoint.y + HALF_CHUNK_SIZE, MODELS_FC_RADIUS) ||
          frustum.isInsideXZ(chunkMidPoint.x + HALF_CHUNK_SIZE, chunkMidPoint.y + HALF_CHUNK_SIZE, MODELS_FC_RADIUS) ||
          frustum.isInsideXZ(chunkMidPoint.x + HALF_CHUNK_SIZE, chunkMidPoint.y - HALF_CHUNK_SIZE, MODELS_FC_RADIUS) ||
          frustum.isInsideXZ(chunkMidPoint.x - HALF_CHUNK_SIZE, chunkMidPoint.y - HALF_CHUNK_SIZE, MODELS_FC_RADIUS))
        {
          addIndirectBufferData(directionToChunkLength,
                                indicesSize,
                                chunks[i].getNumInstances(index),
                                chunks[i].getInstanceOffset(index));
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
}

void Mesh::updateIndirectBufferData()
{
  {
    BENCHMARK("Mesh: bind+buffer indirect data", true);
    basicGLBuffers.bind(VAO | DIBO);
    glBufferSubData(GL_DRAW_INDIRECT_BUFFER, 0, sizeof(GLuint) * 5 * drawIndirectCommandPrimCount, multiDrawIndirectData);
  }
}

void Mesh::addIndirectBufferData(int directionToChunkLength,
                                 GLuint indicesSize,
                                 GLuint numInstances,
                                 GLuint instanceOffset)
{
  indirectTokensSorted.insert(std::pair<int,IndirectBufferToken>(directionToChunkLength, IndirectBufferToken(indicesSize, numInstances, instanceOffset)));
  ++drawIndirectCommandPrimCount;
}
