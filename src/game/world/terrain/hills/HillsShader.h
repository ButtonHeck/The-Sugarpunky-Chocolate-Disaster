#pragma once

#include "GraphicsSettings"

#include <array>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Shader;
class Frustum;

class HillsShader
{
public:
  HillsShader(Shader& renderShader, Shader& cullingShader, Shader& normalsShader);
  void setupCulling();
  void update(const glm::vec3 &lightDir,
              const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
              const glm::mat4 &projectionView,
              const glm::vec3 &viewPosition,
              const Frustum &viewFrustum,
              float maxHillHeight,
              bool useFrustumCulling,
              bool useShadows);
  void updateNormals(const glm::mat4 &projectionView);
  void debugRenderMode(bool enable);

private:
  friend class HillsRenderer;
  Shader& renderShader;
  Shader& cullingShader;
  Shader& normalsShader;
};
