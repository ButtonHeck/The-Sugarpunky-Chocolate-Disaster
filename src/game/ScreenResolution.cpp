#include "ScreenResolution.h"

ScreenResolution::ScreenResolution(int width, int height, float aspect)
  :
    width(width),
    height(height),
    aspectRatio(aspect)
{}

void ScreenResolution::updateResolution(int newWidth, int newHeight)
{
  width = newWidth;
  height = newHeight;
  updateAspectRatio((float)newWidth / (float)newHeight);
}

int ScreenResolution::getWidth() const
{
  return width;
}

int ScreenResolution::getHeight() const
{
  return height;
}

float ScreenResolution::getAspectRatio() const
{
  return aspectRatio;
}

void ScreenResolution::updateAspectRatio(float newAspect)
{
  aspectRatio = newAspect;
}
