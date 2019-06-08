#include "VRAM_Monitor"

VRAM_Monitor::VRAM_Monitor()
{
  glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &ramSize);
  ramSizeFloatPercentage = (float)ramSize / 100;
}

VRAM_Monitor &VRAM_Monitor::getInstance()
{
  static VRAM_Monitor instance;
  return instance;
}

void VRAM_Monitor::updateAvailable()
{
  glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &ramAvailable);
}

int VRAM_Monitor::getAvailableMemory() const
{
  return ramAvailable;
}

float VRAM_Monitor::getAvailableMemoryPercent() const
{
  return ramAvailable / ramSizeFloatPercentage;
}
