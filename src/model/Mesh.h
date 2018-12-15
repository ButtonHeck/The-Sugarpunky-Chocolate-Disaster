#ifndef MESH_H
#define MESH_H
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>

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
  Mesh(Mesh&& old) noexcept;
  Mesh(const Mesh& rhs);
private:
  std::vector<Vertex> vertices;
  std::vector<Texture> textures;
  std::vector<unsigned int> indices;
  friend class Model;
};

#endif // MESH_H
