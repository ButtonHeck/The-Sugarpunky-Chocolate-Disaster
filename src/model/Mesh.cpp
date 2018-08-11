#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices)
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

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

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

  glCreateBuffers(1, &multiDE_I_DIBO);
  glBindBuffer(GL_DRAW_INDIRECT_BUFFER, multiDE_I_DIBO);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::setupInstances(glm::mat4 *models, unsigned int numModels)
{
  numInstances = numModels;
  glBindVertexArray(VAO);
  glCreateBuffers(1, &instanceVBO);
  glNamedBufferStorage(instanceVBO, sizeof(glm::mat4) * numModels, 0, GL_MAP_WRITE_BIT);
  GLfloat* storage = (GLfloat*)glMapNamedBuffer(instanceVBO, GL_WRITE_ONLY);
  std::memcpy(storage, &models[0], sizeof(glm::mat4) * numModels);
  glUnmapNamedBuffer(instanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  for (unsigned int i = 0; i < 4; ++i)
    {
      glEnableVertexAttribArray(i+5);
      glVertexAttribPointer(i+5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
      glVertexAttribDivisor(i+5, 1);
    }
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw(Shader &shader, const glm::vec2 &cameraPositionXZ, std::vector<ModelChunk>& chunks, unsigned int index,
                bool modelRenderOptimize, unsigned int chunkLoadingDistance, Frustum& frustum)
{
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  unsigned int heightNr = 1;
  unsigned int normalNr = 1;

  for(unsigned int i = 0; i < textures.size(); i++)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      std::string number;
      std::string name = textures[i].type;
      if (name == "u_texture_diffuse")
        number = std::to_string(diffuseNr++);
      else if (name == "u_texture_specular")
        number = std::to_string(specularNr++);
      else if (name == "u_texture_normal")
        number = std::to_string(normalNr++);
      else if (name == "u_texture_height")
        number = std::to_string(heightNr++);
      {
        BENCHMARK("Shader: find texture location", true);
        glUniform1i(glGetUniformLocation(shader.getID(), (name + number).c_str()), i);
      }
      glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
  glBindVertexArray(VAO);

  if (modelRenderOptimize)
    {
      GLuint multiDrawIndirectData[chunks.size() * 5]; // { indicesCount, numInstancesToDraw, firstIndex, baseVertex, baseInstance }
      GLuint dataOffset = 0;
      GLuint multiDE_I_primCount = 0;
      GLuint indicesSize = indices.size();
      const float HALF_CHUNK_SIZE = CHUNK_SIZE / 2.0f;
      float radius = HALF_CHUNK_SIZE * glm::sqrt(2);
      for (unsigned int i = 0; i < chunks.size(); i++)
        {
          //if chunk is farther than load distance - just discard it
          glm::vec2 directionToChunk = chunks[i].getMidPoint() - cameraPositionXZ;
          if (glm::length(directionToChunk) > CHUNK_SIZE * chunkLoadingDistance)
            continue;

          glm::vec2 chunkMidPoint = chunks[i].getMidPoint();
          glm::vec2 chunkLL = glm::vec2(chunkMidPoint.x - HALF_CHUNK_SIZE, chunkMidPoint.y + HALF_CHUNK_SIZE);
          if (frustum.isInside(chunkLL.x, 0.0f, chunkLL.y, radius))
            {
              ++multiDE_I_primCount;
              multiDrawIndirectData[dataOffset++] = indicesSize;
              multiDrawIndirectData[dataOffset++] = chunks[i].getNumInstances(index);
              multiDrawIndirectData[dataOffset++] = 0;
              multiDrawIndirectData[dataOffset++] = 0;
              multiDrawIndirectData[dataOffset++] = chunks[i].getInstanceOffset(index);
              continue;
            }
          glm::vec2 chunkLR = glm::vec2(chunkMidPoint.x + HALF_CHUNK_SIZE, chunkMidPoint.y + HALF_CHUNK_SIZE);
          if (frustum.isInside(chunkLR.x, 0.0f, chunkLR.y, radius))
            {
              ++multiDE_I_primCount;
              multiDrawIndirectData[dataOffset++] = indicesSize;
              multiDrawIndirectData[dataOffset++] = chunks[i].getNumInstances(index);
              multiDrawIndirectData[dataOffset++] = 0;
              multiDrawIndirectData[dataOffset++] = 0;
              multiDrawIndirectData[dataOffset++] = chunks[i].getInstanceOffset(index);
              continue;
            }
          glm::vec2 chunkUR = glm::vec2(chunkMidPoint.x + HALF_CHUNK_SIZE, chunkMidPoint.y - HALF_CHUNK_SIZE);
          if (frustum.isInside(chunkUR.x, 0.0f, chunkUR.y, radius))
            {
              ++multiDE_I_primCount;
              multiDrawIndirectData[dataOffset++] = indicesSize;
              multiDrawIndirectData[dataOffset++] = chunks[i].getNumInstances(index);
              multiDrawIndirectData[dataOffset++] = 0;
              multiDrawIndirectData[dataOffset++] = 0;
              multiDrawIndirectData[dataOffset++] = chunks[i].getInstanceOffset(index);
              continue;
            }
          glm::vec2 chunkUL = glm::vec2(chunkMidPoint.x - HALF_CHUNK_SIZE, chunkMidPoint.y - HALF_CHUNK_SIZE);
          if (frustum.isInside(chunkUL.x, 0.0f, chunkUL.y, radius))
            {
              ++multiDE_I_primCount;
              multiDrawIndirectData[dataOffset++] = indicesSize;
              multiDrawIndirectData[dataOffset++] = chunks[i].getNumInstances(index);
              multiDrawIndirectData[dataOffset++] = 0;
              multiDrawIndirectData[dataOffset++] = 0;
              multiDrawIndirectData[dataOffset++] = chunks[i].getInstanceOffset(index);
            }
        }
      glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(GLuint) * 5 * multiDE_I_primCount, multiDrawIndirectData, GL_STATIC_DRAW);
      glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, multiDE_I_primCount, 0);
    }
  else
    {
      glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, numInstances);
    }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
