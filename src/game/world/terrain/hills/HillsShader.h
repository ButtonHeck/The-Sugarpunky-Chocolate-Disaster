#ifndef HILLSSHADER_H
#define HILLSSHADER_H
#include "graphics/Shader.h"
#include "graphics/Frustum.h"

class HillsShader
{
public:
  HillsShader(Shader& renderShader, Shader& cullingShader);
  void setupCulling();
  void update(bool useFC,
              bool useShadows,
              glm::mat4& projectionView,
              glm::vec3& viewPosition,
              Frustum& viewFrustum,
              float maxHillHeight);
private:
  friend class HillsRenderer;
  Shader& renderShader;
  Shader& cullingShader;
};

#endif // HILLSSHADER_H
