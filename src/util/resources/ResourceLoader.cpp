/*
 * Copyright 2019 Ilya Malgin
 * ResourceLoader.cpp
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
 * Purpose: contains definitions of the ResourceLoader namespace functions
 * @version 0.1.0
 */

#include "ResourceLoader"
#include "TextureResourceLoader"
#include "ShaderResourceLoader"
#include "ModelResourceLoader"

/**
* @brief delegates initialize command to all loader subsystems
* @note model loading should be done after textures have been loaded
*/
void ResourceLoader::initialize()
{
	TextureResourceLoader::initialize( "data\\textures.sprd" );
	ShaderResourceLoader::initialize( "data\\shaders.sprd" );
	ModelResourceLoader::initialize( "data\\models.sprd" );
}

/**
* @brief delegates release command to all loader subsystems
*/
void ResourceLoader::release()
{
	TextureResourceLoader::release();
	ShaderResourceLoader::release();
	ModelResourceLoader::release();
}
