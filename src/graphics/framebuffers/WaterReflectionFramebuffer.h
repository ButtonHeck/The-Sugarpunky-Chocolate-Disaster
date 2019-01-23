#ifndef WATERREFLECTIONFRAMEBUFFER_H
#define WATERREFLECTIONFRAMEBUFFER_H
#include "graphics/openglObjects/Framebuffer.h"

class WaterReflectionFramebuffer : public Framebuffer
{
public:
  WaterReflectionFramebuffer(TextureManager& textureManager);
  virtual ~WaterReflectionFramebuffer() = default;
  virtual void setup() override;
};

#endif // WATERREFLECTIONFRAMEBUFFER_H
