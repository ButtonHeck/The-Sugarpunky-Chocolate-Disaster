#pragma once

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
