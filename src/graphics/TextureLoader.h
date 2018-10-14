#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include <IL/il.h>
#include <GL/glew.h>
#include <string>
#include <vector>
#include <glm/common.hpp>
#include <memory>
#include "graphics/ScreenResolution.h"
#include "game/world/terrain/WaterGenerator.h"

class TextureLoader
{
public:
  TextureLoader(ScreenResolution& screenResolution);
  GLuint loadTexture(const std::string& path, GLuint textureUnit, GLenum wrapType, GLint magFilter, GLint minFilter, bool useAnisotropy, bool includeCWD = true);
  GLuint createFrameMSTexture(int multisample, GLuint textureUnit);
  GLuint createFrameTexture(GLuint textureUnit);
  GLuint createDepthMapTexture(int width, int height, GLuint textureUnit);
  GLuint loadCubemap(const std::string& directory, GLuint textureUnit);
  GLuint createUnderwaterReliefTexture(const std::shared_ptr<WaterGenerator> waterMapGenerator, GLuint textureUnit, GLint magFilter, GLint minFilter);
  unsigned int getMaxMip(unsigned int width, unsigned int height);
private:
  GLuint createAndBindTextureObject(GLenum target, GLuint textureUnit);
  void setTexture2DParameters(GLint magFilter, GLint minFilter, GLenum wrapType);
  ScreenResolution& screenResolution;
};

#endif // TEXTURELOADER_H
