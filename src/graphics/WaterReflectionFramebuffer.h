#ifndef WATERREFLECTIONFRAMEBUFFER_H
#define WATERREFLECTIONFRAMEBUFFER_H
#include <GL/glew.h>
#include "util/Logger.h"
#include "graphics/TextureManager.h"

class WaterReflectionFramebuffer
{
public:
  WaterReflectionFramebuffer(TextureManager& textureManager);
  void bindToViewport(int viewportWidth, int viewportHeight);
  void unbindToViewport(int viewportWidth, int viewportHeight);
  virtual ~WaterReflectionFramebuffer();
  void setup();
private:
  TextureManager& textureManager;
  GLuint fbo;
};

#endif // WATERREFLECTIONFRAMEBUFFER_H
