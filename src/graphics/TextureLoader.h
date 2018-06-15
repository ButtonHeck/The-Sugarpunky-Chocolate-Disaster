#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include <IL/il.h>
#include <GL/glew.h>
#include <string>
#include <vector>
#include <glm/common.hpp>
#include "src/game/Settings.h"
#include "src/generator/WaterMapGenerator.h"

class TextureLoader
{
public:
  TextureLoader();
  GLuint loadTexture(const std::string& path, GLuint textureUnit, GLenum wrapType, GLint magFilter, GLint minFilter, bool includeCWD = true);
  GLuint createFrameHDRTexture(int width, int height, GLuint textureUnit);
  GLuint createFrameMSTexture(int width, int height, int multisample, GLuint textureUnit);
  GLuint createFrameTexture(int width, int height, GLuint textureUnit);
  GLuint createDepthMapTexture(int width, int height, GLuint textureUnit);
  GLuint loadCubemap(const std::string& directory, GLuint textureUnit);
  GLuint createUnderwaterReliefTexture(WaterMapGenerator* waterMapGenerator, GLuint textureUnit, GLint magFilter, GLint minFilter);
};

#endif // TEXTURELOADER_H
