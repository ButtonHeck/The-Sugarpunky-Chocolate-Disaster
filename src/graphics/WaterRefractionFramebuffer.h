#ifndef WATERREFRACTIONFRAMEBUFFER_H
#define WATERREFRACTIONFRAMEBUFFER_H
#include <GL/glew.h>
#include "util/Logger.h"
#include "graphics/TextureManager.h"

class WaterRefractionFramebuffer
{
public:
  WaterRefractionFramebuffer(int width, int height, TextureManager& textureManager);
  void bind();
  void unbind();
  virtual ~WaterRefractionFramebuffer();
  void setup();
private:
  int width, height;
  TextureManager& textureManager;
  GLuint fbo;
};

#endif // WATERREFRACTIONFRAMEBUFFER_H
