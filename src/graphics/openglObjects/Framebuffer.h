#pragma once

#include <GL/glew.h>

class TextureManager;

class Framebuffer
{
public:
  Framebuffer(TextureManager& textureManager) noexcept;
  virtual ~Framebuffer();
  virtual void setup() = 0;
  void checkStatus();
  void bindToViewport(int viewportWidth, int viewportHeight) noexcept;
  void unbindToViewport(int viewportWidth, int viewportHeight) noexcept;

protected:
  GLuint fbo;
  TextureManager& textureManager;
};
