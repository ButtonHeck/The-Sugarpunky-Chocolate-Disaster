#ifndef WATERSHADER_H
#define WATERSHADER_H
#include "graphics/Shader.h"
#include "graphics/Frustum.h"

class WaterShader
{
public:
  WaterShader(Shader& renderShader, Shader& cullingShader, Shader& normalsShader);
  void setupCulling();
  void update(glm::vec3 &lightDir,
              glm::mat4 &lightSpaceMatrixNear,
              glm::mat4 &lightSpaceMatrixFar,
              bool useFC,
              glm::mat4& projectionView,
              glm::vec3& viewPosition,
              Frustum& viewFrustum);
  void updateNormals(glm::mat4& projectionView);
  void debugRenderMode(bool enable);
private:
  friend class WaterRenderer;
  Shader& renderShader;
  Shader& cullingShader;
  Shader& normalsShader;
};

#endif // WATERSHADER_H
