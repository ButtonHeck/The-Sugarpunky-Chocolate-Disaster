#ifndef WATERSHADER_H
#define WATERSHADER_H
#include "graphics/Shader.h"
#include "graphics/Frustum.h"

class WaterShader
{
public:
  WaterShader(Shader& renderShader, Shader& cullingShader, Shader& normalsShader);
  void setupCulling();
  void update(bool useFC, glm::mat4& projectionView, glm::vec3& viewPosition, Frustum& viewFrustum);
  void updateNormals(glm::mat4& projectionView);
  void debugRenderMode(bool enable);
private:
  friend class WaterRenderer;
  Shader& renderShader;
  Shader& cullingShader;
  Shader& normalsShader;
};

#endif // WATERSHADER_H
