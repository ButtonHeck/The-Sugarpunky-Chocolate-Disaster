#ifndef DEPTHMAPFRAMEBUFFER_H
#define DEPTHMAPFRAMEBUFFER_H
#include "Framebuffer"

class DepthmapFramebuffer : public Framebuffer
{
public:
  DepthmapFramebuffer(TextureManager& textureManager);
  virtual ~DepthmapFramebuffer() = default;
  virtual void setup() override;
};

#endif // DEPTHMAPFRAMEBUFFER_H
