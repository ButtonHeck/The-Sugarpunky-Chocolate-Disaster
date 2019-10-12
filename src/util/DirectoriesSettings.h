/*
 * Copyright 2019 Ilya Malgin
 * DirectoriesSettings.h
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
 * Purpose: contains project-relative paths to different kinds of application resources and declares a function
 * to get project's path
 * @version 0.1.0
 */

#pragma once

#include <string>

const std::string getResourcesDirectory();
const std::string RES_DIR = getResourcesDirectory() + "/res/";
const std::string TEXTURES_DIR = RES_DIR + "textures/";
const std::string MODELS_DIR = RES_DIR + "models/";
const std::string SAVES_DIR = RES_DIR + "saves/";
