#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include <IL/il.h>
#include <GL/glew.h>
#include <string>
#include <vector>
#include "Settings.h"
#include "WaterMapGenerator.h"

class TextureLoader
{
public:
  TextureLoader();
  GLuint loadTexture(const std::string& path, GLuint textureUnit, GLenum wrapType, GLint magFilter, GLint minFilter);
  GLuint loadCubemap(std::vector<std::string>& faces, GLuint textureUnit);
  GLuint createUnderwaterReliefTexture(WaterMapGenerator* waterMapGenerator, GLuint textureUnit, GLint magFilter, GLint minFilter);
};

#endif // TEXTURELOADER_H
