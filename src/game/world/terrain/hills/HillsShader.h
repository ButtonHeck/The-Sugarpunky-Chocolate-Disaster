#ifndef HILLSSHADER_H
#define HILLSSHADER_H
#include "graphics/Shader.h"
#include "graphics/Frustum.h"

class HillsShader
{
public:
  HillsShader(Shader& renderShader, Shader& cullingShader, Shader& normalsShader);
  void setupCulling();
  void update(bool useFC,
              bool useShadows,
              glm::mat4& projectionView,
              glm::vec3& viewPosition,
              Frustum& viewFrustum,
              float maxHillHeight);
  void updateNormals(glm::mat4& projectionView);
  void debugRenderMode(bool enable);
private:
  friend class HillsRenderer;
  Shader& renderShader;
  Shader& cullingShader;
  Shader& normalsShader;
};

#endif // HILLSSHADER_H
