/*
 * Copyright 2019 Ilya Malgin
 * ScreenResolution.h
 * This file is part of The Sugarpunky Chocolate Disaster project
 *
 * The Sugarpunky Chocolate Disaster project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Sugarpunky Chocolate Disaster project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * See <http://www.gnu.org/licenses/>
 *
 * Purpose: contains declaration for ScreenResolution class
 * @version 0.1.0
 */

#pragma once

/**
* @brief Utility class representing screen resolution. 
* Responsible for holding resolution parameters: width, height and ratio
*/
class ScreenResolution
{
public:
  constexpr static float REFERENCE_WIDTH = 1440.0f;
  constexpr static float REFERENCE_HEIGHT = 900.0f;

  ScreenResolution(int width, int height) noexcept;
  void updateResolution(int newWidth, int newHeight) noexcept;
  int getWidth() const noexcept;
  int getHeight() const noexcept;
  float getAspectRatio() const noexcept;
  float getWidthRatioToReference() const noexcept;
  float getHeightRatioToReference() const noexcept;

private:
  int width, height;
  float aspectRatio;
};
