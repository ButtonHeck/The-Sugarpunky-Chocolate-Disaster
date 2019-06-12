#pragma once

#include "GraphicsSettings"

#include <array>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Frustum;
class Shader;

class WaterShader
{
public:
  WaterShader(Shader& renderShader, Shader& cullingShader, Shader& normalsShader) noexcept;
  void setupCulling();
  void update(const glm::vec3 &lightDir,
              const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
              const glm::mat4 &projectionView,
              const glm::vec3 &viewPosition,
              const Frustum &viewFrustum,
              bool useFrustumCulling);
  void updateNormals(const glm::mat4 &projectionView);
  void debugRenderMode(bool enable);

private:
  constexpr static float DUDV_SPEED = 0.0004f;
  friend class WaterRenderer;

  Shader& renderShader;
  Shader& cullingShader;
  Shader& normalsShader;
};
