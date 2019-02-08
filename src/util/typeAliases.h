#ifndef TYPEALIASES_H
#define TYPEALIASES_H
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

using map2D_f = std::vector<std::vector<float>>;
using map2D_i = std::vector<std::vector<int>>;
using map2D_vec3 = std::vector<std::vector<glm::vec3>>;
using map2D_mat4 = std::vector<std::vector<glm::mat4>>;
template<typename T> using map2D_template = std::vector<std::vector<T>>;
template<typename T> using vec2D_template = std::vector<std::vector<T>>;

#endif // TYPEALIASES_H
