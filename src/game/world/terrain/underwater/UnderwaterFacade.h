#pragma once

#include "UnderwaterGenerator"
#include "UnderwaterShader"
#include "UnderwaterRenderer"

class UnderwaterFacade
{
public:
  UnderwaterFacade(Shader& renderShader) noexcept;
  void draw(const glm::vec3 &lightDir, const glm::mat4 &projectionView);

private:
  UnderwaterShader shader;
  UnderwaterGenerator generator;
  UnderwaterRenderer renderer;
};
