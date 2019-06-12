#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Shader;

class UnderwaterShader
{
public:
  UnderwaterShader(Shader& renderShader) noexcept;
  void update(const glm::vec3 &lightDir, const glm::mat4 &projectionView);

private:
  Shader& renderShader;
};
