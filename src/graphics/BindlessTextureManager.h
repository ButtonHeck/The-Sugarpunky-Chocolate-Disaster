#ifndef BINDLESSTEXTUREMANAGER_H
#define BINDLESSTEXTUREMANAGER_H
#include <GL/glew.h>
#include <vector>
#include <string>
//#include "graphics/Shader.h"

struct BindlessTexture
{
  BindlessTexture(const std::string& samplerUniformName, GLuint id, GLuint64 handle);
  std::string samplerUniformName;
  GLuint id;
  GLuint64 handle;
};

class Shader;

class BindlessTextureManager
{
public:
  BindlessTextureManager() = delete;
  static void emplace_back(const std::string& textureSamplerUniformName, GLuint textureID, GLuint64 textureHandle);
  static void loadToShaders(Shader& phongShader, Shader& gouraudShader);
  static void makeAllNonResident();
private:
  static std::vector<BindlessTexture> textures;
};

#endif // BINDLESSTEXTUREMANAGER_H
