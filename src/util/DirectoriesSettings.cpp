/*
 * Copyright 2019 Ilya Malgin
 * DirectoriesSettings.cpp
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
 * Purpose: contains definition of a function to get project's path
 * @version 0.1.0
 */

#include "DirectoriesSettings"

#ifdef _WIN32
#include <direct.h>
#elif defined __linux__
#include <unistd.h>
#endif

/**
* @brief helper function to get CWD in runtime
*/
const std::string getResourcesDirectory()
{
	char cwd_buffer[FILENAME_MAX];
	getcwd( cwd_buffer, FILENAME_MAX );
	std::string cwd( cwd_buffer );
	return cwd.substr( 0, cwd.find_last_of( "/" ) );
}
