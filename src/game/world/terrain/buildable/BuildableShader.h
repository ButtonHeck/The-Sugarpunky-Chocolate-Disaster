#ifndef BUIDABLESHADER_H
#define BUIDABLESHADER_H
#include "graphics/shaders/Shader.h"

class BuildableShader
{
public:
  BuildableShader(Shader& buildableRenderShader, Shader& selectedRenderShader);
  void updateBuildable(glm::mat4& projectionView);
  void updateSelected(glm::mat4 &projectionView, glm::mat4 &selectedModel);
private:
  Shader& buildableRenderShader;
  Shader& selectedRenderShader;
};

#endif // BUIDABLESHADER_H
