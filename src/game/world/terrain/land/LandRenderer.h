#ifndef LANDRENDERER_H
#define LANDRENDERER_H
#include "game/world/terrain/land/LandGenerator.h"
#include "util/BenchmarkTimer.h"

class LandRenderer
{
public:
  LandRenderer(LandGenerator& generator);
  void render();

private:
  LandGenerator& generator;
};

#endif // LANDRENDERER_H
