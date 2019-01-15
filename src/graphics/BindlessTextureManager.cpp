#include "BindlessTextureManager.h"

std::vector<BindlessTexture> BindlessTextureManager::modelTextures;
void BindlessTextureManager::emplaceBackModelTexture(const std::string &textureSamplerUniformName, GLuint textureID, GLuint64 textureHandle)
{
  static bool capacityReserved = false;
  if (!capacityReserved)
    {
      modelTextures.reserve(200);
      capacityReserved = true;
    }
  modelTextures.emplace_back(textureSamplerUniformName, textureID, textureHandle);
}

std::vector<BindlessTexture> BindlessTextureManager::lensFlareTextures;
void BindlessTextureManager::emplaceBackLensFlareTexture(const std::string &textureSamplerUniformName, GLuint textureID, GLuint64 textureHandle)
{
  static bool capacityReserved = false;
  if (!capacityReserved)
    {
      lensFlareTextures.reserve(10);
      capacityReserved = true;
    }
  lensFlareTextures.emplace_back(textureSamplerUniformName, textureID, textureHandle);
}

void BindlessTextureManager::loadToModelShaders(Shader &phongShader, Shader &gouraudShader)
{
  for (BindlessTexture& texture : modelTextures)
    {
      glMakeTextureHandleResidentARB(texture.handle);
      phongShader.use();
      phongShader.setUInt64(texture.samplerUniformName, texture.handle);
      gouraudShader.use();
      gouraudShader.setUInt64(texture.samplerUniformName, texture.handle);
    }
}

void BindlessTextureManager::loadToLensFlareShader(Shader &shader)
{
  shader.use();
  for (BindlessTexture& texture : lensFlareTextures)
    {
      glMakeTextureHandleResidentARB(texture.handle);
      shader.setUInt64(texture.samplerUniformName, texture.handle);
    }
}

void BindlessTextureManager::makeAllNonResident()
{
  for (BindlessTexture& texture : modelTextures)
    glMakeTextureHandleNonResidentARB(texture.handle);
  for (BindlessTexture& texture : lensFlareTextures)
    glMakeTextureHandleNonResidentARB(texture.handle);
}

BindlessTexture::BindlessTexture(const std::string &textureSamplerUniformName, GLuint textureID, GLuint64 handle)
  :
    samplerUniformName(textureSamplerUniformName),
    id(textureID),
    handle(handle)
{}
