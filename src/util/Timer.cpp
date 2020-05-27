/*
 * Copyright 2019 Ilya Malgin
 * Timer.cpp
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
 * Purpose: contains definitions for Timer class
 * @version 0.1.0
 */

#include "Timer"

#include <GLFW/glfw3.h>

/**
* @brief plain ctor, initializes frame time with the timestamp of this constructor invocation
*/
Timer::Timer() noexcept
	: frameTime( chronoClock::now() )
	, currentTime( frameTime )
	, frames( 0 )
	, fps( 0 )
	, updateCount( 0 )
{}

/**
* @brief updates frame timestamps and FPS value.
* In addition, every second sends command to benchmark timer to process frame benchmarks
* @todo try to get rid of GLFW timing function
*/
float Timer::tick()
{
	//update timestamps
	nowTime = glfwGetTime();
	delta = nowTime - lastTime;
	lastTime = nowTime;
	++frames;
	currentTime = chronoClock::now();

	//update FPS value
	if( std::chrono::duration_cast<std::chrono::milliseconds>( currentTime - frameTime ).count() > 1000 )
	{
		frameTime = currentTime;
		fps = frames;
		frames = 0;
		++updateCount;
	}
	return delta;
}

unsigned int Timer::getFPS() noexcept
{
	return fps;
}
