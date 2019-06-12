#pragma once

#include "TypeAliases"

#include <GL/glew.h>

class ScreenResolution;

class TextureLoader
{
public:
  TextureLoader(const ScreenResolution &screenResolution) noexcept;
  GLuint loadTexture(const std::string& path,
                     GLuint textureUnit,
                     GLenum wrapType,
                     GLint magFilter,
                     GLint minFilter,
                     bool useAnisotropy,
                     bool includeCWD = true,
                     bool isBindless = false,
                     bool explicitNoSRGB = false);
  GLuint createFrameMSTexture(GLuint textureUnit, int multisamples) noexcept;
  GLuint createFrameTexture(GLuint textureUnit, bool isDepthTexture, bool useAnisotropy) noexcept;
  GLuint createFrameTextureSized(GLuint textureUnit, bool isDepthTexture, int width, int height, bool useAnisotropy) noexcept;
  GLuint createDepthMapTexture(GLuint textureUnit, int width, int height) noexcept;
  GLuint loadCubemap(const std::string& directory, GLuint textureUnit, bool explicitNoSRGB);
  GLuint createUnderwaterReliefTexture(GLuint textureUnit, const map2D_f& waterMap, GLint magFilter, GLint minFilter);

private:
  GLuint createTextureObject(GLenum target, GLuint textureUnit, bool isBindless) noexcept;
  void setTexture2DParameters(GLuint texture, GLint magFilter, GLint minFilter, GLenum wrapType) noexcept;
  void setTex2DArrayParameters(GLint magFilter, GLint minFilter, GLenum wrapType) noexcept;

  const ScreenResolution& screenResolution;
};
