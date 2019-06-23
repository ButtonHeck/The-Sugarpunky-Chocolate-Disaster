/*
 * Copyright 2019 Ilya Malgin
 * VRAM_Monitor.h
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
 * Purpose: contains declaration for VRAM_Monitor class
 * @version 0.1.0
 */

#pragma once

/**
* @brief utility class responsible for updating available VRAM information
*/
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
