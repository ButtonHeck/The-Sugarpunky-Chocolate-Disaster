#ifndef BUIDABLESHADER_H
#define BUIDABLESHADER_H
#include "../../src/graphics/shaders/Shader.h"

class BuildableShader
{
public:
  BuildableShader(Shader& buildableRenderShader, Shader& selectedRenderShader);
  void updateBuildable(const glm::mat4 &projectionView);
  void updateSelected(const glm::mat4 &projectionView, const glm::vec4 &selectedTranslation);

private:
  Shader& buildableRenderShader;
  Shader& selectedRenderShader;
};

#endif // BUIDABLESHADER_H
