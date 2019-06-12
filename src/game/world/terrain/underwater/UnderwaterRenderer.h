#pragma once

class UnderwaterGenerator;

class UnderwaterRenderer
{
public:
  UnderwaterRenderer(UnderwaterGenerator& generator) noexcept;
  void render();

private:
  UnderwaterGenerator& generator;
};
