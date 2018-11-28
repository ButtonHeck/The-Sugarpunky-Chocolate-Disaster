#ifndef LANDSHADER_H
#define LANDSHADER_H
#include "graphics/Shader.h"

class LandShader
{
public:
  LandShader(Shader& renderShader);
  void update(glm::vec3 &lightDir,
              glm::mat4 &lightSpaceMatrixNear,
              glm::mat4 &lightSpaceMatrixFar,
              glm::mat4& projectionView,
              glm::vec3 &viewPosition,
              bool useShadows);
private:
  Shader& renderShader;
};

#endif // LANDSHADER_H
