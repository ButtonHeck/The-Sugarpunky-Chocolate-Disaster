#ifndef UNDERWATERFACADE_H
#define UNDERWATERFACADE_H
#include "game/world/terrain/underwater/UnderwaterGenerator.h"
#include "game/world/terrain/underwater/UnderwaterShader.h"
#include "game/world/terrain/underwater/UnderwaterRenderer.h"
#include "util/BenchmarkTimer.h"

class UnderwaterFacade
{
public:
  UnderwaterFacade(Shader& renderShader);
  void draw(glm::mat4 &projectionView);
private:
  UnderwaterShader shader;
  UnderwaterGenerator generator;
  UnderwaterRenderer renderer;
};

#endif // UNDERWATERFACADE_H
