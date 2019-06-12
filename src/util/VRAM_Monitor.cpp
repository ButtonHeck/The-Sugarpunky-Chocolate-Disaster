#include "VRAM_Monitor"
#include <GL/glew.h>

VRAM_Monitor::VRAM_Monitor() noexcept
{
  glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &ramSize);
  ramSizeFloatPercentage = (float)ramSize / 100;
}

VRAM_Monitor &VRAM_Monitor::getInstance() noexcept
{
  static VRAM_Monitor instance;
  return instance;
}

void VRAM_Monitor::updateAvailable() noexcept
{
  glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &ramAvailable);
}

int VRAM_Monitor::getAvailableMemory() const noexcept
{
  return ramAvailable;
}

float VRAM_Monitor::getAvailableMemoryPercent() const noexcept
{
  return ramAvailable / ramSizeFloatPercentage;
}
