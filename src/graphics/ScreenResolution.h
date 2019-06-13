#pragma once

class ScreenResolution
{
public:
  constexpr static float REFERENCE_WIDTH = 1440.0f;
  constexpr static float REFERENCE_HEIGHT = 900.0f;

  ScreenResolution() = default;
  ScreenResolution(int width, int height) noexcept;
  void updateResolution(int newWidth, int newHeight) noexcept;
  int getWidth() const noexcept;
  int getHeight() const noexcept;
  float getAspectRatio() const noexcept;
  float getWidthRatioToReference() const noexcept;
  float getHeightRatioToReference() const noexcept;

private:
  void updateAspectRatio(float newAspect) noexcept;

  int width = 0, height = 0;
  float aspectRatio = 1.0f;
};
