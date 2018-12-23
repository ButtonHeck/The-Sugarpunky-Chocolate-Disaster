#include "BindlessTextureManager.h"

std::vector<BindlessTexture> BindlessTextureManager::textures;
void BindlessTextureManager::emplace_back(const std::string &textureSamplerUniformName, GLuint textureID, GLuint64 textureHandle)
{
  static bool capacityReserved = false;
  if (!capacityReserved)
    {
      textures.reserve(200);
      capacityReserved = true;
    }
  textures.emplace_back(textureSamplerUniformName, textureID, textureHandle);
}

void BindlessTextureManager::loadToShaders(Shader &phongShader, Shader &gouraudShader)
{
  for (BindlessTexture& texture : textures)
    {
      glMakeTextureHandleResidentARB(texture.handle);
      phongShader.use();
      phongShader.setUInt64(texture.samplerUniformName, texture.handle);
      gouraudShader.use();
      gouraudShader.setUInt64(texture.samplerUniformName, texture.handle);
    }
}

void BindlessTextureManager::makeAllNonResident()
{
  for (BindlessTexture& texture : textures)
    glMakeTextureHandleNonResidentARB(texture.handle);
}

BindlessTexture::BindlessTexture(const std::string &textureSamplerUniformName, GLuint textureID, GLuint64 handle)
  :
    samplerUniformName(textureSamplerUniformName),
    id(textureID),
    handle(handle)
{}
