#ifndef LANDRENDERER_H
#define LANDRENDERER_H
#include "LandGenerator.h"
#include "../../src/util/BenchmarkTimer.h"

class LandRenderer
{
public:
  LandRenderer(LandGenerator& generator);
  void render();

private:
  LandGenerator& generator;
};

#endif // LANDRENDERER_H
