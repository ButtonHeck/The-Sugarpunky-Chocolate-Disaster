#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<GLuint> indices)
{
  this->vertices = vertices;
  this->textures = textures;
  this->indices = indices;
  setupMesh();
}

void Mesh::setupMesh()
{
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
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

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

  glCreateBuffers(1, &multiDE_I_DIBO);
  glBindBuffer(GL_DRAW_INDIRECT_BUFFER, multiDE_I_DIBO);
  glNamedBufferStorage(multiDE_I_DIBO, sizeof(multiDrawIndirectData), 0, GL_DYNAMIC_STORAGE_BIT);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::setupInstances(glm::mat4 *models, unsigned int numModels)
{
  numInstances = numModels;
  glBindVertexArray(VAO);
  if (instanceVBO != 0)
    {
      glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
      glInvalidateBufferData(instanceVBO);
      glDeleteBuffers(1, &instanceVBO);
    }
  glCreateBuffers(1, &instanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
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

void Mesh::draw(bool useCulling, bool bindTexture, bool updateIndirect)
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
  glBindVertexArray(VAO);
  if (useCulling)
    {
      {
        BENCHMARK("Mesh: bind+buffer indirect data", true);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, multiDE_I_DIBO);
        if (updateIndirect)
          glBufferSubData(GL_DRAW_INDIRECT_BUFFER, 0, sizeof(GLuint) * 5 * multiDE_I_primCount, multiDrawIndirectData);
      }
      {
        BENCHMARK("Mesh: multiDrawIndirect", true);
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, multiDE_I_primCount, 0);
      }
    }
  else
    glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, numInstances);
}

void Mesh::prepareIndirectBufferData(std::vector<ModelChunk>& chunks,
                                     unsigned int index,
                                     const glm::vec2 &cameraPositionXZ,
                                     Frustum& frustum)
{
  BENCHMARK("(ST)Mesh: update DIB data", true);
  multiDE_I_primCount = 0;
  indirectTokensSorted.clear();
  GLuint indicesSize = indices.size();
  for (unsigned int i = 0; i < chunks.size(); i++)
    {
      //if chunk is farther than load distance - just discard it
      glm::vec2 directionToChunk = chunks[i].getMidPoint() - cameraPositionXZ;
      unsigned int directionToChunkLength = glm::length2(directionToChunk);
      if (directionToChunkLength > LOADING_DISTANCE_UNITS_SQUARE)
        continue;

      glm::vec2 chunkMidPoint = chunks[i].getMidPoint();
      if (frustum.isInsideXZ(chunkMidPoint.x - HALF_CHUNK_SIZE,
                           chunkMidPoint.y + HALF_CHUNK_SIZE,
                           MODELS_FC_RADIUS))
        {
          addIndirectBufferData(directionToChunkLength,
                                indicesSize,
                                chunks[i].getNumInstances(index),
                                chunks[i].getInstanceOffset(index));
          continue;
        }
      if (frustum.isInsideXZ(chunkMidPoint.x + HALF_CHUNK_SIZE,
                           chunkMidPoint.y + HALF_CHUNK_SIZE,
                           MODELS_FC_RADIUS))
        {
          addIndirectBufferData(directionToChunkLength,
                                indicesSize,
                                chunks[i].getNumInstances(index),
                                chunks[i].getInstanceOffset(index));
          continue;
        }
      if (frustum.isInsideXZ(chunkMidPoint.x + HALF_CHUNK_SIZE,
                           chunkMidPoint.y - HALF_CHUNK_SIZE,
                           MODELS_FC_RADIUS))
        {
          addIndirectBufferData(directionToChunkLength,
                                indicesSize,
                                chunks[i].getNumInstances(index),
                                chunks[i].getInstanceOffset(index));
          continue;
        }
      if (frustum.isInsideXZ(chunkMidPoint.x - HALF_CHUNK_SIZE,
                           chunkMidPoint.y - HALF_CHUNK_SIZE,
                           MODELS_FC_RADIUS))
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

void Mesh::addIndirectBufferData(int directionToChunkLength,
                                 GLuint indicesSize,
                                 GLuint numInstances,
                                 GLuint instanceOffset)
{
  indirectTokensSorted.insert(std::pair<int,IndirectBufferToken>(directionToChunkLength, IndirectBufferToken(indicesSize, numInstances, instanceOffset)));
  ++multiDE_I_primCount;
}
