#ifndef WATERREFRACTIONFRAMEBUFFER_H
#define WATERREFRACTIONFRAMEBUFFER_H
#include <GL/glew.h>
#include "util/Logger.h"
#include "graphics/TextureManager.h"

class WaterRefractionFramebuffer
{
public:
  WaterRefractionFramebuffer(TextureManager& textureManager);
  void bindToViewport(int viewportWidth, int viewportHeight);
  void unbindToViewport(int viewportWidth, int viewportHeight);
  virtual ~WaterRefractionFramebuffer();
  void setup();
private:
  TextureManager& textureManager;
  GLuint fbo;
};

#endif // WATERREFRACTIONFRAMEBUFFER_H
