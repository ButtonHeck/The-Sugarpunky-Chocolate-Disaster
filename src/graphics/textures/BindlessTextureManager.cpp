/*
 * Copyright 2019 Ilya Malgin
 * BindlessTextureManager.cpp
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
 * Purpose: contains definition for BindlessTextureManager class and BindlessTexture struct
 * @version 0.1.0
 */

#include "BindlessTextureManager"
#include "Shader"

//static variable definition
vec2D_template<BindlessTexture> BindlessTextureManager::textures;

/**
* @brief create bindless texture object of a given type. Additionally reserves textures storage capacity if need
* @param textureSamplerUniformName name of a texture used as a sampler name in a shader
* @param textureID ID of a texture in OpenGL
* @param textureType semantic type of a texture
*/
void BindlessTextureManager::emplaceBack(const std::string &textureSamplerUniformName, GLuint textureID, BINDLESS_TEXTURE_TYPE textureType)
{
  static bool capacityReserved = false;
  if (!capacityReserved)
    {
      textures.reserve(BINDLESS_TEXTURE_NUM_TYPES);
      for (unsigned int textureTypeIndex = 0; textureTypeIndex < BINDLESS_TEXTURE_NUM_TYPES; textureTypeIndex++)
        {
          std::vector<BindlessTexture> emptyVec;
          textures.push_back(emptyVec);
          textures[textureTypeIndex].reserve(INITIAL_CAPACITY);
        }
      capacityReserved = true;
    }
  GLuint64 textureHandle = glGetTextureHandleARB(textureID);
  textures[textureType].emplace_back(textureSamplerUniformName, textureID, textureHandle);
}

/**
* @brief send command to OpenGL to make all the textures resident in memory.
* Should be used when all textures have been loaded and before game loop begins
*/
void BindlessTextureManager::makeAllResident()
{
  for (unsigned int textureTypeIndex = 0; textureTypeIndex < textures.size(); textureTypeIndex++)
    {
      for (BindlessTexture& texture : textures[textureTypeIndex])
        glMakeTextureHandleResidentARB(texture.handle);
    }
}

/**
* @brief set textures handles for a given type to a given shader as uniform values
* @param shader shader that uses bindless textures of a given type
* @param textureType semantic type of a texture. It is programmer's responsibility to make sure that given type is correct for the shader
*/
void BindlessTextureManager::loadToShader(Shader &shader, BINDLESS_TEXTURE_TYPE textureType)
{
  shader.use();
  for (BindlessTexture& texture : textures.at(textureType))
    shader.setUint64(texture.samplerUniformName.c_str(), texture.handle);
}

/**
* @brief send command to OpenGL to make all the textures non-resident in memory. Additionally sends delete command for each texture.
* Should be used when the app is about to close
*/
void BindlessTextureManager::makeAllNonResident()
{
  for (unsigned int textureTypeIndex = 0; textureTypeIndex < textures.size(); textureTypeIndex++)
    {
      for (BindlessTexture& texture : textures[textureTypeIndex])
        {
          glMakeTextureHandleNonResidentARB(texture.handle);
          glDeleteTextures(1, &texture.id);
        }
    }
}

/**
* @brief creates a bindless texture object with all necessary params
* @param textureSamplerUniformName name of a texture used as a sampler name in a shader
* @param textureID ID of a texture in OpenGL
* @param handle 64-bit handle promoted to OpenGL as a pointer to the actual residental texture  
*/
BindlessTexture::BindlessTexture(const std::string &textureSamplerUniformName, GLuint textureID, GLuint64 handle)
  :
    samplerUniformName(textureSamplerUniformName),
    id(textureID),
    handle(handle)
{}
