#pragma once

#include <GL/glew.h>

class VRAM_Monitor final
{
public:
  static VRAM_Monitor& getInstance();
  void updateAvailable();
  int getAvailableMemory() const;
  float getAvailableMemoryPercent() const;
private:
  VRAM_Monitor();
  int ramAvailable, ramSize;
  float ramSizeFloatPercentage;
};
