#include "graphics/textures/BindlessTextureManager.h"

std::vector<std::vector<BindlessTexture>> BindlessTextureManager::textures;
void BindlessTextureManager::emplaceBack(const std::string &textureSamplerUniformName, GLuint textureID, BINDLESS_TEXTURE_TYPE textureType)
{
  static bool capacityReserved = false;
  if (!capacityReserved)
    {
      textures.reserve(BINDLESS_TEXTURE_NUM_TYPES);
      for (unsigned int i = 0; i < BINDLESS_TEXTURE_NUM_TYPES; i++)
        {
          std::vector<BindlessTexture> emptyVec;
          textures.push_back(emptyVec);
          textures[i].reserve(256);
        }
      capacityReserved = true;
    }
  GLuint64 textureHandle = glGetTextureHandleARB(textureID);
  if (textureType == BINDLESS_TEXTURE_MODEL)
    textures[BINDLESS_TEXTURE_MODEL].emplace_back(textureSamplerUniformName, textureID, textureHandle);
  else if (textureType == BINDLESS_TEXTURE_LENS_FLARE)
    textures[BINDLESS_TEXTURE_LENS_FLARE].emplace_back(textureSamplerUniformName, textureID, textureHandle);
}

void BindlessTextureManager::makeAllResident()
{
  for (unsigned int i = 0; i < textures.size(); i++)
    {
      for (BindlessTexture& texture : textures[i])
        glMakeTextureHandleResidentARB(texture.handle);
    }
}

void BindlessTextureManager::loadToShader(Shader &shader, BINDLESS_TEXTURE_TYPE textureType)
{
  shader.use();
  for (BindlessTexture& texture : textures.at(textureType))
    shader.setUint64(texture.samplerUniformName, texture.handle);
}

void BindlessTextureManager::makeAllNonResident()
{
  for (unsigned int i = 0; i < textures.size(); i++)
    {
      for (BindlessTexture& texture : textures[i])
        glMakeTextureHandleNonResidentARB(texture.handle);
    }
}

BindlessTexture::BindlessTexture(const std::string &textureSamplerUniformName, GLuint textureID, GLuint64 handle)
  :
    samplerUniformName(textureSamplerUniformName),
    id(textureID),
    handle(handle)
{}
