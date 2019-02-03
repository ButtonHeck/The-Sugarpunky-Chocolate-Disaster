#ifndef BUIDABLESHADER_H
#define BUIDABLESHADER_H
#include "graphics/shaders/Shader.h"

class BuildableShader
{
public:
  BuildableShader(Shader& buildableRenderShader, Shader& selectedRenderShader);
  void updateBuildable(const glm::mat4 &projectionView);
  void updateSelected(const glm::mat4 &projectionView, const glm::mat4 &selectedModel);
private:
  Shader& buildableRenderShader;
  Shader& selectedRenderShader;
};

#endif // BUIDABLESHADER_H
