/*
 * Copyright 2019 Ilya Malgin
 * BindlessTextureManager.h
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
 * Purpose: contains declaration for BindlessTextureManager class and BindlessTexture struct
 * @version 0.1.0
 */

#pragma once

#include "TypeAliases"

#include <GL/glew.h>

class Shader;

enum BINDLESS_TEXTURE_TYPE
{
  BINDLESS_TEXTURE_MODEL,
  BINDLESS_TEXTURE_LENS_FLARE,
  BINDLESS_TEXTURE_NUM_TYPES
};

/**
* @brief representation of a bindless texture
*/
struct BindlessTexture
{
  BindlessTexture(const std::string& samplerUniformName, GLuint id, GLuint64 handle);
  std::string samplerUniformName;
  GLuint id;
  GLuint64 handle;
};

/**
* @brief manager of bindless textures in the app. Responsible for keeping all the textures in the dedicated storage,
* sending them to shaders and for residency handling.
*/
class BindlessTextureManager
{
public:
  BindlessTextureManager() = delete;
  static void emplaceBack(const std::string& textureSamplerUniformName, GLuint textureID, BINDLESS_TEXTURE_TYPE textureType);
  static void makeAllResident();
  static void loadToShader(Shader& shader, BINDLESS_TEXTURE_TYPE textureType);
  static void makeAllNonResident();

private:
  constexpr static unsigned int INITIAL_CAPACITY = 256;
  static vec2D_template<BindlessTexture> textures;
};
