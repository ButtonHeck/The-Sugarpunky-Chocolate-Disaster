#ifndef WATERREFRACTIONFRAMEBUFFER_H
#define WATERREFRACTIONFRAMEBUFFER_H
#include "Framebuffer"

class WaterRefractionFramebuffer : public Framebuffer
{
public:
  WaterRefractionFramebuffer(TextureManager& textureManager);
  virtual ~WaterRefractionFramebuffer() = default;
  virtual void setup() override;
};

#endif // WATERREFRACTIONFRAMEBUFFER_H
