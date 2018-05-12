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

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::setupInstances(glm::mat4 *models, unsigned int numModels)
{
  numInstances = numModels;
  glBindVertexArray(VAO);
  glGenBuffers(1, &instanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * numModels, &models[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), 0);
  glEnableVertexAttribArray(6);
  glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(sizeof(glm::vec4)));
  glEnableVertexAttribArray(7);
  glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
  glEnableVertexAttribArray(8);
  glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
  glVertexAttribDivisor(5, 1);
  glVertexAttribDivisor(6, 1);
  glVertexAttribDivisor(7, 1);
  glVertexAttribDivisor(8, 1);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw(Shader &shader, Camera &camera, std::vector<ModelChunk>& chunks, unsigned int index,
                bool modelRenderOptimize, unsigned int chunkLoadingDistance)
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
      if (name == "texture_diffuse")
        number = std::to_string(diffuseNr++);
      else if (name == "texture_specular")
        number = std::to_string(specularNr++);
      else if (name == "texture_normal")
        number = std::to_string(normalNr++);
      else if (name == "texture_height")
        number = std::to_string(heightNr++);
      glUniform1i(glGetUniformLocation(shader.getID(), (name + number).c_str()), i);
      glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
  glBindVertexArray(VAO);

  float cameraOnMapX = camera.getPosition().x;
  float cameraOnMapZ = camera.getPosition().z;
  cameraOnMapX = glm::clamp(cameraOnMapX, -TILES_WIDTH/2.0f, TILES_WIDTH/2.0f);
  cameraOnMapZ = glm::clamp(cameraOnMapZ, -TILES_HEIGHT/2.0f, TILES_HEIGHT/2.0f);
  int cameraOnMapCoordX = (int)(TILES_WIDTH + cameraOnMapX) - TILES_WIDTH / 2;
  cameraOnMapCoordX = glm::clamp(cameraOnMapCoordX, 0, TILES_WIDTH - 1);
  int cameraOnMapCoordZ = (int)(TILES_HEIGHT + cameraOnMapZ) - TILES_HEIGHT / 2;
  cameraOnMapCoordZ = glm::clamp(cameraOnMapCoordZ, 0, TILES_HEIGHT - 1);
  glm::vec2 cameraPosition = glm::vec2(cameraOnMapX, cameraOnMapZ);
  glm::vec2 viewDirection = glm::vec2(camera.getDirection().x, camera.getDirection().z);
  viewDirection = glm::fastNormalize(viewDirection);
  float cameraCorrectedFOVDOT = FOV_DOT_PRODUCT - camera.getPosition().y / 70.0f;

  if (modelRenderOptimize)
    {
      for (unsigned int i = 0; i < chunks.size(); i++)
        {
          if (chunks[i].containsPoint(cameraOnMapCoordX, cameraOnMapCoordZ))
            {
              glDrawElementsInstancedBaseInstance(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0,
                                                  chunks[i].getNumInstances(index), chunks[i].getInstanceOffset(index));
              continue;
            }
          glm::vec2 directionToChunk = chunks[i].getMidPoint() - cameraPosition;
          if (glm::fastLength(directionToChunk) > MODEL_CHUNK_SIZE * chunkLoadingDistance)
            continue;
          glm::vec2 directionToChunkUL = glm::vec2(chunks[i].getLeft() - 192.0f, chunks[i].getTop() - 192.0f) - cameraPosition;
          glm::vec2 directionToChunkUR = glm::vec2(chunks[i].getRight() - 192.0f, chunks[i].getTop() - 192.0f) - cameraPosition;
          glm::vec2 directionToChunkLR = glm::vec2(chunks[i].getRight() - 192.0f, chunks[i].getBottom() - 192.0f) - cameraPosition;
          glm::vec2 directionToChunkLL = glm::vec2(chunks[i].getLeft() - 192.0f, chunks[i].getBottom() - 192.0f) - cameraPosition;
          directionToChunkUL = glm::fastNormalize(directionToChunkUL);
          directionToChunkUR = glm::fastNormalize(directionToChunkUR);
          directionToChunkLR = glm::fastNormalize(directionToChunkLR);
          directionToChunkLL = glm::fastNormalize(directionToChunkLL);
          if (glm::dot(directionToChunkUL, viewDirection) > cameraCorrectedFOVDOT ||
              glm::dot(directionToChunkUR, viewDirection) > cameraCorrectedFOVDOT ||
              glm::dot(directionToChunkLR, viewDirection) > cameraCorrectedFOVDOT ||
              glm::dot(directionToChunkLL, viewDirection) > cameraCorrectedFOVDOT)
            glDrawElementsInstancedBaseInstance(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0,
                                                chunks[i].getNumInstances(index), chunks[i].getInstanceOffset(index));
        }
    }
  else
    {
      glDrawElementsInstancedBaseInstance(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, numInstances, 0);
    }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
