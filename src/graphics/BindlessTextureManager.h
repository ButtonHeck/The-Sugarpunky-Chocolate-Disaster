#ifndef BINDLESSTEXTUREMANAGER_H
#define BINDLESSTEXTUREMANAGER_H
#include <GL/glew.h>
#include <vector>
#include <string>
#include "graphics/Shader.h"

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
  static void emplaceBackModelTexture(const std::string& textureSamplerUniformName, GLuint textureID, GLuint64 textureHandle);
  static void emplaceBackLensFlareTexture(const std::string& textureSamplerUniformName, GLuint textureID, GLuint64 textureHandle);
  static void loadToModelShaders(Shader& phongShader, Shader& gouraudShader);
  static void loadToLensFlareShader(Shader& shader);
  static void makeAllNonResident();
private:
  static std::vector<BindlessTexture> modelTextures;
  static std::vector<BindlessTexture> lensFlareTextures;
};

#endif // BINDLESSTEXTUREMANAGER_H
