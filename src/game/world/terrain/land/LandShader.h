#pragma once

#include "GraphicsSettings"

#include <array>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Shader;

class LandShader
{
public:
  LandShader(Shader& renderShader) noexcept;
  void update(const glm::vec3 &lightDir,
              const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
              const glm::mat4 &projectionView,
              bool useShadows);

private:
  Shader& renderShader;
};
