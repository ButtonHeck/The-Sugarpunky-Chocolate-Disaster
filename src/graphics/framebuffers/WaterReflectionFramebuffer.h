#ifndef WATERREFLECTIONFRAMEBUFFER_H
#define WATERREFLECTIONFRAMEBUFFER_H
#include "Framebuffer"

class WaterReflectionFramebuffer : public Framebuffer
{
public:
  WaterReflectionFramebuffer(TextureManager& textureManager);
  virtual ~WaterReflectionFramebuffer();
  virtual void setup() override;

private:
  GLuint rbo;
};

#endif // WATERREFLECTIONFRAMEBUFFER_H
