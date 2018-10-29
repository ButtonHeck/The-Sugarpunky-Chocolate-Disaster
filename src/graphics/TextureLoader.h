#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include <string>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <IL/il.h>
#include "graphics/ScreenResolution.h"
#include "util/Settings.h"
#include "util/Logger.h"

class TextureLoader
{
public:
  TextureLoader(ScreenResolution& screenResolution);
  GLuint loadTexture(const std::string& path, GLuint textureUnit, GLenum wrapType, GLint magFilter, GLint minFilter, bool useAnisotropy, bool includeCWD = true);
  GLuint createFrameMSTexture(int multisample, GLuint textureUnit);
  GLuint createFrameTexture(GLuint textureUnit);
  GLuint createDepthMapTexture(int width, int height, GLuint textureUnit);
  GLuint loadCubemap(const std::string& directory, GLuint textureUnit);
  GLuint createUnderwaterReliefTexture(std::vector<std::vector<float>>& waterMap, GLuint textureUnit, GLint magFilter, GLint minFilter);
  unsigned int getMaxMip(unsigned int width, unsigned int height);
private:
  GLuint createAndBindTextureObject(GLenum target, GLuint textureUnit);
  void setTexture2DParameters(GLint magFilter, GLint minFilter, GLenum wrapType);
  ScreenResolution& screenResolution;
};

#endif // TEXTURELOADER_H
