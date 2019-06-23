/*
 * Copyright 2019 Ilya Malgin
 * TextureManager.h
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
 * Purpose: contains declaration for TextureManager class
 * @version 0.1.0
 */

#pragma once

#include "TextureUnits"
#include "TypeAliases"

#include <GL/glew.h>
#include <unordered_map>

class TextureLoader;

/**
* @brief manager for all non-bindless textures in the game, responsible for keeping all textures IDs in one storage and
* initializing them with appropriate parameters
*/
class TextureManager
{
public:
  TextureManager(TextureLoader& loader);
  virtual ~TextureManager();
  void createUnderwaterReliefTexture(const map2D_f &waterMap);
  GLuint& get(int textureUnit);
  TextureLoader& getLoader() noexcept;

private:
  TextureLoader& loader;
  std::unordered_map<int, GLuint> textures;
};
