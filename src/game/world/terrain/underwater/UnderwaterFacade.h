#pragma once

#include "UnderwaterGenerator"
#include "UnderwaterShader"
#include "UnderwaterRenderer"
#include "BenchmarkTimer"

class UnderwaterFacade
{
public:
  UnderwaterFacade(Shader& renderShader);
  void draw(const glm::vec3 &lightDir, const glm::mat4 &projectionView);

private:
  UnderwaterShader shader;
  UnderwaterGenerator generator;
  UnderwaterRenderer renderer;
};
