#ifndef HILLSSHADER_H
#define HILLSSHADER_H
#include "graphics/Shader.h"
#include "graphics/Frustum.h"

class HillsShader
{
public:
  HillsShader(Shader& renderShader, Shader& cullingShader, Shader& normalsShader);
  void setupCulling();
  void update(glm::vec3& lightDir,
              glm::mat4& lightSpaceMatrix,
              glm::mat4& projectionView,
              glm::vec3& viewPosition,
              Frustum& viewFrustum,
              float maxHillHeight,
              bool useFC,
              bool useShadows);
  void updateNormals(glm::mat4& projectionView);
  void debugRenderMode(bool enable);
private:
  friend class HillsRenderer;
  Shader& renderShader;
  Shader& cullingShader;
  Shader& normalsShader;
};

#endif // HILLSSHADER_H
