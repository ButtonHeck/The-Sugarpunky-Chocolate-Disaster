/*
 * Copyright 2019 Ilya Malgin
 * RendererState.cpp
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
 * Purpose: contains definitions for OpenGL render related functions
 * @version 0.1.0
 */

#include "RendererState"

#include <GL/glew.h>

namespace RendererState
{
	/**
	* @brief setup initial OpenGL state
	* @param useMultisample indicator of multisampling mode for rendering
	*/
	void setInitialRenderingState( bool useMultisample ) noexcept
	{
		glEnable( GL_CULL_FACE );
		glEnable( GL_DEPTH_TEST );
		glDisable( GL_DITHER );
		if( useMultisample )
		{
			glEnable( GL_MULTISAMPLE );
		}
		else
		{
			glDisable( GL_MULTISAMPLE );
		}
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	}

	/**
	* @brief set OpenGL rendering state for ambience type data (sun, lens flares etc.)
	* @param isOn flag for ambience state mode
	*/
	void setAmbienceRenderingState( bool isOn ) noexcept
	{
		if( isOn )
		{
			glFrontFace( GL_CW );
			glDepthFunc( GL_LEQUAL );
			glEnable( GL_BLEND );
			glDepthMask( GL_FALSE );
		}
		else
		{
			glDepthMask( GL_TRUE );
			glDisable( GL_BLEND );
			glDepthFunc( GL_LESS );
			glFrontFace( GL_CCW );
		}
	}
}
