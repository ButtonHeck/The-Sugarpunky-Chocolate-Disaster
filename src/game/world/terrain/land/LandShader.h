#ifndef LANDSHADER_H
#define LANDSHADER_H
#include "graphics/shaders/Shader.h"

class LandShader
{
public:
  LandShader(Shader& renderShader);
  void update(glm::vec3 &lightDir,
              const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
              glm::mat4& projectionView,
              bool useShadows);
private:
  Shader& renderShader;
};

#endif // LANDSHADER_H
