#ifndef WATERREFRACTIONFRAMEBUFFER_H
#define WATERREFRACTIONFRAMEBUFFER_H
#include "../../src/graphics/openglObjects/Framebuffer.h"

class WaterRefractionFramebuffer : public Framebuffer
{
public:
  WaterRefractionFramebuffer(TextureManager& textureManager);
  virtual ~WaterRefractionFramebuffer() = default;
  virtual void setup() override;
};

#endif // WATERREFRACTIONFRAMEBUFFER_H
