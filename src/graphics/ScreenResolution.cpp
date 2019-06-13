#include "ScreenResolution"

ScreenResolution::ScreenResolution(int width, int height) noexcept
  :
    width(width),
    height(height),
    aspectRatio((float)width / (float)height)
{}

void ScreenResolution::updateResolution(int newWidth, int newHeight) noexcept
{
  width = newWidth;
  height = newHeight;
  updateAspectRatio((float)newWidth / (float)newHeight);
}

int ScreenResolution::getWidth() const noexcept
{
  return width;
}

int ScreenResolution::getHeight() const noexcept
{
  return height;
}

float ScreenResolution::getAspectRatio() const noexcept
{
  return aspectRatio;
}

float ScreenResolution::getWidthRatioToReference() const noexcept
{
  return width / REFERENCE_WIDTH;
}

float ScreenResolution::getHeightRatioToReference() const noexcept
{
  return height / REFERENCE_HEIGHT;
}

void ScreenResolution::updateAspectRatio(float newAspect) noexcept
{
  aspectRatio = newAspect;
}
