#ifndef LANDSHADER_H
#define LANDSHADER_H
#include "Shader"

class LandShader
{
public:
  LandShader(Shader& renderShader);
  void update(const glm::vec3 &lightDir,
              const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
              const glm::mat4 &projectionView,
              bool useShadows);

private:
  Shader& renderShader;
};

#endif // LANDSHADER_H
