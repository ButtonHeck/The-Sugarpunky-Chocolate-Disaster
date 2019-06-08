#include "BindlessTextureManager"

vec2D_template<BindlessTexture> BindlessTextureManager::textures;
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

void BindlessTextureManager::makeAllResident()
{
  for (unsigned int textureTypeIndex = 0; textureTypeIndex < textures.size(); textureTypeIndex++)
    {
      for (BindlessTexture& texture : textures[textureTypeIndex])
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
  for (unsigned int textureTypeIndex = 0; textureTypeIndex < textures.size(); textureTypeIndex++)
    {
      for (BindlessTexture& texture : textures[textureTypeIndex])
        {
          glMakeTextureHandleNonResidentARB(texture.handle);
          glDeleteTextures(1, &texture.id);
        }
    }
}

BindlessTexture::BindlessTexture(const std::string &textureSamplerUniformName, GLuint textureID, GLuint64 handle)
  :
    samplerUniformName(textureSamplerUniformName),
    id(textureID),
    handle(handle)
{}
