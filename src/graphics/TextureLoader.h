#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include <IL/il.h>
#include <GL/glew.h>
#include <string>
#include <vector>
#include "src/game/Settings.h"
#include "src/generator/WaterMapGenerator.h"

class TextureLoader
{
public:
  TextureLoader();
  GLuint loadTexture(const std::string& path, GLuint textureUnit, GLenum wrapType, GLint magFilter, GLint minFilter, bool includeCWD = true);
  GLuint loadCubemap(const std::string& directory, GLuint textureUnit);
  GLuint createUnderwaterReliefTexture(WaterMapGenerator* waterMapGenerator, GLuint textureUnit, GLint magFilter, GLint minFilter);
};

#endif // TEXTURELOADER_H
