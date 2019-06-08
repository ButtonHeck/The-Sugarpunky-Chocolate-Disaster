#pragma once

#include "Generator"

class UnderwaterGenerator : public Generator
{
public:
  UnderwaterGenerator();
  virtual ~UnderwaterGenerator() = default;
private:
  friend class UnderwaterRenderer;
};
