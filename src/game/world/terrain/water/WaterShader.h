#ifndef WATERSHADER_H
#define WATERSHADER_H
#include "graphics/shaders/Shader.h"
#include "graphics/Frustum.h"

class WaterShader
{
public:
  WaterShader(Shader& renderShader, Shader& cullingShader, Shader& normalsShader);
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
  friend class WaterRenderer;
  Shader& renderShader;
  Shader& cullingShader;
  Shader& normalsShader;
};

#endif // WATERSHADER_H
