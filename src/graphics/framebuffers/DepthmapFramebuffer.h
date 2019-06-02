#ifndef DEPTHMAPFRAMEBUFFER_H
#define DEPTHMAPFRAMEBUFFER_H
#include "../../src/graphics/openglObjects/Framebuffer.h"

class DepthmapFramebuffer : public Framebuffer
{
public:
  DepthmapFramebuffer(TextureManager& textureManager);
  virtual ~DepthmapFramebuffer() = default;
  virtual void setup() override;
};

#endif // DEPTHMAPFRAMEBUFFER_H
