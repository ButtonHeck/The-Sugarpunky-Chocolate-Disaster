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

void Mesh::draw(Shader &shader, bool useCulling, bool bindTexture)
{  
  BENCHMARK("Mesh: draw (full func)", true);
  if (bindTexture)
    {
      if (textures.size() == 1) //for mostly all models we have only one texture(diffuse)
        {
          glActiveTexture(GL_TEXTURE0);
          //dont need to get uniform location
          glBindTexture(GL_TEXTURE_2D, textures[0].id);
        }
      else
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
                BENCHMARK("Shader: mesh find texture location", true);
                glUniform1i(glGetUniformLocation(shader.getID(), (name.append(number)).c_str()), i);
              }
              glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }
        }
    }
  glBindVertexArray(VAO);
  if (useCulling)
    {
      {
        BENCHMARK("Mesh: bind+buffer indirect data", true);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, multiDE_I_DIBO);
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
  GLuint dataOffset = 0;
  multiDE_I_primCount = 0;
  GLuint indicesSize = indices.size();
  for (unsigned int i = 0; i < chunks.size(); i++)
    {
      //if chunk is farther than load distance - just discard it
      glm::vec2 directionToChunk = chunks[i].getMidPoint() - cameraPositionXZ;
      if (glm::length2(directionToChunk) > LOADING_DISTANCE_UNITS_SQUARE)
        continue;

      glm::vec2 chunkMidPoint = chunks[i].getMidPoint();
      if (frustum.isInsideXZ(chunkMidPoint.x - HALF_CHUNK_SIZE,
                           chunkMidPoint.y + HALF_CHUNK_SIZE,
                           MODELS_FC_RADIUS))
        {
          addIndirectBufferData(dataOffset, indicesSize, chunks[i].getNumInstances(index), chunks[i].getInstanceOffset(index));
          continue;
        }
      if (frustum.isInsideXZ(chunkMidPoint.x + HALF_CHUNK_SIZE,
                           chunkMidPoint.y + HALF_CHUNK_SIZE,
                           MODELS_FC_RADIUS))
        {
          addIndirectBufferData(dataOffset, indicesSize, chunks[i].getNumInstances(index), chunks[i].getInstanceOffset(index));
          continue;
        }
      if (frustum.isInsideXZ(chunkMidPoint.x + HALF_CHUNK_SIZE,
                           chunkMidPoint.y - HALF_CHUNK_SIZE,
                           MODELS_FC_RADIUS))
        {
          addIndirectBufferData(dataOffset, indicesSize, chunks[i].getNumInstances(index), chunks[i].getInstanceOffset(index));
          continue;
        }
      if (frustum.isInsideXZ(chunkMidPoint.x - HALF_CHUNK_SIZE,
                           chunkMidPoint.y - HALF_CHUNK_SIZE,
                           MODELS_FC_RADIUS))
        {
          addIndirectBufferData(dataOffset, indicesSize, chunks[i].getNumInstances(index), chunks[i].getInstanceOffset(index));
        }
    }
}

void Mesh::addIndirectBufferData(GLuint& dataOffset,
                                 GLuint indicesSize,
                                 GLuint numInstances,
                                 GLuint instanceOffset)
{
  ++multiDE_I_primCount;
  multiDrawIndirectData[dataOffset++] = indicesSize;
  multiDrawIndirectData[dataOffset++] = numInstances;
  multiDrawIndirectData[dataOffset++] = 0;
  multiDrawIndirectData[dataOffset++] = 0;
  multiDrawIndirectData[dataOffset++] = instanceOffset;
}
