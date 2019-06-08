#pragma once

#include "UnderwaterGenerator"

class UnderwaterRenderer
{
public:
  UnderwaterRenderer(UnderwaterGenerator& generator);
  void render();
private:
  UnderwaterGenerator& generator;
};
