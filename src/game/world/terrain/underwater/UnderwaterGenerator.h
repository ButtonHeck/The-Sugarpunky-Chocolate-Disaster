#pragma once

#include "Generator"

class UnderwaterGenerator : public Generator
{
public:
  UnderwaterGenerator() noexcept;
  virtual ~UnderwaterGenerator() = default;

private:
  friend class UnderwaterRenderer;
};
