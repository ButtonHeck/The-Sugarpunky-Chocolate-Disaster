#include "model/Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices)
  :
    vertices(vertices),
    textures(textures),
    indices(indices)
{}

Mesh::Mesh(Mesh &&old) noexcept
  :
    vertices(old.vertices),
    textures(old.textures),
    indices(old.indices)
{}

Mesh::Mesh(const Mesh &rhs)
  :
    vertices(rhs.vertices),
    textures(rhs.textures),
    indices(rhs.indices)
{}
