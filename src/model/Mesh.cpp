#include "model/Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
  :
    vertices(vertices),
    indices(indices)
{}

Mesh::Mesh(Mesh &&old) noexcept
  :
    vertices(old.vertices),
    indices(old.indices)
{}

Mesh::Mesh(const Mesh &rhs)
  :
    vertices(rhs.vertices),
    indices(rhs.indices)
{}
