#ifndef LANDRENDERER_H
#define LANDRENDERER_H
#include "LandGenerator"
#include "BenchmarkTimer"

class LandRenderer
{
public:
  LandRenderer(LandGenerator& generator);
  void render();

private:
  LandGenerator& generator;
};

#endif // LANDRENDERER_H
