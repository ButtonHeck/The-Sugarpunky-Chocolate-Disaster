#pragma once

class LandGenerator;

class LandRenderer
{
public:
  LandRenderer(LandGenerator& generator);
  void render();

private:
  LandGenerator& generator;
};
