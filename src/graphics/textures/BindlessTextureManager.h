#pragma once

#include "Shader"
#include "TypeAliases"

#include <GL/glew.h>

enum BINDLESS_TEXTURE_TYPE
{
  BINDLESS_TEXTURE_MODEL,
  BINDLESS_TEXTURE_LENS_FLARE,
  BINDLESS_TEXTURE_NUM_TYPES
};

struct BindlessTexture
{
  BindlessTexture(const std::string& samplerUniformName, GLuint id, GLuint64 handle);
  std::string samplerUniformName;
  GLuint id;
  GLuint64 handle;
};

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
