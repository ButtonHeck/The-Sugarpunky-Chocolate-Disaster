/*
 * Copyright 2019 Ilya Malgin
 * ShaderManager.h
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
 * Purpose: contains declaration for ShaderManager class
 * @version 0.1.0
 */

#pragma once

#include "ShaderUnits"

#include <unordered_map>

class ScreenResolution;
class Shader;

/**
* @brief manager for all shader programs in the game.
* Responsible for holding shader programs and initializing their 'constant' uniforms
* (the ones that just set once at the beginning)
*/
class ShaderManager
{
public:
	ShaderManager() noexcept;
	virtual ~ShaderManager();
	void setupConstantUniforms( const ScreenResolution & screenResolution );
	Shader & get( SHADER_UNIT type );

private:
	std::unordered_map<int, Shader> shaders;
};
