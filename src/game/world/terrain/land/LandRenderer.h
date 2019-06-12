#pragma once

class LandGenerator;

class LandRenderer
{
public:
  LandRenderer(LandGenerator& generator) noexcept;
  void render();

private:
  LandGenerator& generator;
};
