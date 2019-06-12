#pragma once

class VRAM_Monitor final
{
public:
  static VRAM_Monitor& getInstance() noexcept;
  void updateAvailable() noexcept;
  int getAvailableMemory() const noexcept;
  float getAvailableMemoryPercent() const noexcept;

private:
  VRAM_Monitor() noexcept;
  int ramAvailable, ramSize;
  float ramSizeFloatPercentage;
};
