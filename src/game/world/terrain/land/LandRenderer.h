#pragma once

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
