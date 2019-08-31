/*
 * Copyright 2019 Ilya Malgin
 * Timer.h
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
 * Purpose: contains declaration for Timer class
 * @version 0.1.0
 */

#pragma once

#include <chrono>

/**
* @brief utility class representing game timer, responsible for frame timing manipulations
*/
class Timer
{
public:
	Timer() noexcept;
	float tick();
	unsigned int getFPS() noexcept;

private:
	using chronoClock = std::chrono::high_resolution_clock;

	float lastTime;
	float nowTime;
	float delta;
	decltype( chronoClock::now() ) frameTime;
	decltype( frameTime ) currentTime;
	unsigned int frames;
	unsigned int fps;
	unsigned int updateCount;
};
