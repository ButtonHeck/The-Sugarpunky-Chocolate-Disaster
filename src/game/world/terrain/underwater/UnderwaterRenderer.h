#ifndef UNDERWATERRENDERER_H
#define UNDERWATERRENDERER_H
#include "game/world/terrain/underwater/UnderwaterGenerator.h"

class UnderwaterRenderer
{
public:
  UnderwaterRenderer(UnderwaterGenerator& generator);
  void render();
private:
  UnderwaterGenerator& generator;
};

#endif // UNDERWATERRENDERER_H