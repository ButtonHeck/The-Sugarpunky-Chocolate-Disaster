/*
 * Copyright 2019 Ilya Malgin
 * RendererState.h
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
 * Purpose: contains declarations for OpenGL render related functions
 * @version 0.1.0
 */

#pragma once

#include <initializer_list>

/**
* @brief contains a bunch of OpenGL render related functions
*/
namespace RendererState
{
	void setInitialRenderingState( bool useMultisample ) noexcept;
	void setAmbienceRenderingState( bool isOn ) noexcept;
	void enableStates( const std::initializer_list<unsigned int> & states ) noexcept;
	void disableStates( const std::initializer_list<unsigned int> & states ) noexcept;
};
