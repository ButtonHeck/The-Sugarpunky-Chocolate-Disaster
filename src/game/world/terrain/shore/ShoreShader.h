#pragma once

#include "GraphicsSettings"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <array>

class Shader;

class ShoreShader
{
public:
  ShoreShader(Shader& renderShader, Shader &normalsShader) noexcept;
  void update(const glm::vec3 &lightDir,
              const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
              const glm::mat4 &projectionView,
              bool useShadows,
              bool useClipDistanceReflection,
              bool useClipDistanceRefraction);
  void updateNormals(const glm::mat4 &projectionView);
  void debugRenderMode(bool enable);

private:
  Shader& renderShader;
  Shader& normalsShader;
};
