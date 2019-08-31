/*
 * Copyright 2019 Ilya Malgin
 * Logger.h
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
 * Purpose: contains declarations of logging functions
 * @version 0.1.0
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

namespace Logger
{
	void log( const char * msg );

	/**
	* @brief prints log message to console
	* @param pattern string pattern including wildcard symbols
	* @param token templated data for wildcard
	* @param rest other data for wildcarded format (for recursive parameterized calls)
	*/
	template <typename T, typename... Args>
	void log( const char * pattern, 
			  T token, 
			  Args... rest )
	{
		std::string message = std::string( pattern ).append( token );
		for( ; *pattern != '\0'; pattern++ )
		{
			if( *pattern == '%' )
			{
				std::cout << token;
				log( pattern + 1, rest... );
				return;
			}
			std::cout << *pattern;
		}
	}
	void APIENTRY glDebugCallback( GLenum source,
								   GLenum type,
								   GLuint id,
								   GLenum severity,
								   GLsizei,
								   const GLchar * glMessage,
								   const void * );
};
