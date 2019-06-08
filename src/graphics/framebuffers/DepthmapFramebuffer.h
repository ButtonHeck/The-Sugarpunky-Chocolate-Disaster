#pragma once

#include "Framebuffer"

class DepthmapFramebuffer : public Framebuffer
{
public:
  DepthmapFramebuffer(TextureManager& textureManager);
  virtual ~DepthmapFramebuffer() = default;
  virtual void setup() override;
};
