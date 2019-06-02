#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <GL/glew.h>
#include "../../src/graphics/textures/TextureManager.h"

class Framebuffer
{
public:
  Framebuffer(TextureManager& textureManager);
  virtual ~Framebuffer();
  virtual void setup() = 0;
  void checkStatus();
  void bindToViewport(int viewportWidth, int viewportHeight);
  void unbindToViewport(int viewportWidth, int viewportHeight);

protected:
  GLuint fbo;
  TextureManager& textureManager;
};

#endif // FRAMEBUFFER_H
