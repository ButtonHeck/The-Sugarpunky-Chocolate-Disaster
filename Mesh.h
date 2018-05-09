#ifndef MESH_H
#define MESH_H
#include <Shader.h>
#include <vector>
#include <glm/glm.hpp>
#include "Settings.h"
#include "ModelChunk.h"

struct Vertex
{
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
  glm::vec3 Tangent;
  glm::vec3 Bitangent;
};

struct Texture
{
  unsigned int id;
  std::string type;
  std::string path;
};

class Mesh
{
public:
  Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices);
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
  void setupMesh();
  void setupInstances(glm::mat4* models, unsigned int numModels);
  void draw(Shader& shader, const glm::vec2& cameraPosition, std::vector<ModelChunk>& chunks);
  unsigned int VAO;
private:
  std::vector<Vertex> vertices;
  std::vector<Texture> textures;
  std::vector<unsigned int> indices;
  unsigned int VBO, EBO, instanceVBO;
  unsigned int numInstances;
};

#endif // MESH_H
