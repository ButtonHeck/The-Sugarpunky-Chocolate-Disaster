#ifndef UNDERWATERFACADE_H
#define UNDERWATERFACADE_H
#include "UnderwaterGenerator.h"
#include "UnderwaterShader.h"
#include "UnderwaterRenderer.h"
#include "../../src/util/BenchmarkTimer.h"

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

#endif // UNDERWATERFACADE_H
