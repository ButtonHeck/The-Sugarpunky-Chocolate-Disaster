#ifndef HILLSSHADER_H
#define HILLSSHADER_H
#include "../../src/graphics/shaders/Shader.h"
#include "../../src/graphics/Frustum.h"

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

#endif // HILLSSHADER_H
