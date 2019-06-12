#pragma once

#include "Framebuffer"

class WaterRefractionFramebuffer : public Framebuffer
{
public:
  WaterRefractionFramebuffer(TextureManager& textureManager) noexcept;
  virtual ~WaterRefractionFramebuffer() = default;
  virtual void setup() override;
};
