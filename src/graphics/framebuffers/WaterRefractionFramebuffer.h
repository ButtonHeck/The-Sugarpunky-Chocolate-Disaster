#pragma once

#include "Framebuffer"

class WaterRefractionFramebuffer : public Framebuffer
{
public:
  WaterRefractionFramebuffer(TextureManager& textureManager);
  virtual ~WaterRefractionFramebuffer() = default;
  virtual void setup() override;
};
