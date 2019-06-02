#ifndef SHORESHADER_H
#define SHORESHADER_H
#include "../../src/graphics/shaders/Shader.h"

class ShoreShader
{
public:
  ShoreShader(Shader& renderShader, Shader &normalsShader);
  void update(const glm::vec3 &lightDir,
              const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
              const glm::mat4 &projectionView,
              bool useShadows,
              bool useClipDistanceReflection,
              bool useClipDistanceRefraction);
  void updateNormals(const glm::mat4 &projectionView);
  void debugRenderMode(bool enable);

private:
  Shader& renderShader;
  Shader& normalsShader;
};

#endif // SHORESHADER_H
