#ifndef WATERREFLECTIONFRAMEBUFFER_H
#define WATERREFLECTIONFRAMEBUFFER_H
#include <GL/glew.h>
#include "util/Logger.h"
#include "graphics/TextureManager.h"

class WaterReflectionFramebuffer
{
public:
  WaterReflectionFramebuffer(int width, int height, TextureManager& textureManager);
  void bind();
  void unbind();
  virtual ~WaterReflectionFramebuffer();
  void setup();
private:
  int width, height;
  TextureManager& textureManager;
  GLuint fbo, rbo;
};

#endif // WATERREFLECTIONFRAMEBUFFER_H
