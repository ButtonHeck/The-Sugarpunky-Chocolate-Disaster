/*
 * Copyright 2019 Ilya Malgin
 * VRAM_Monitor.cpp
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
 * Purpose: contains definitions for VRAM_Monitor class
 * @version 0.1.0
 */

#include "VRAM_Monitor"
#include <GL/glew.h>

/**
* @brief initializes member variables by requesting data from OpenGL directly
*/
VRAM_Monitor::VRAM_Monitor() noexcept
{
	glGetIntegerv( GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &ramSize );
	ramSizeFloatPercentage = (float)ramSize / 100;
}

/**
* @brief returns instance of a monitor
*/
VRAM_Monitor & VRAM_Monitor::getInstance() noexcept
{
	static VRAM_Monitor instance;
	return instance;
}

/**
* @brief sends get integer value call to OpenGL to update current available memory size info
*/
void VRAM_Monitor::updateAvailable() noexcept
{
	glGetIntegerv( GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &ramAvailable );
}

int VRAM_Monitor::getAvailableMemory() const noexcept
{
	return ramAvailable;
}

/**
* @brief returns percent value of available memory
*/
float VRAM_Monitor::getAvailableMemoryPercent() const noexcept
{
	return ramAvailable / ramSizeFloatPercentage;
}
