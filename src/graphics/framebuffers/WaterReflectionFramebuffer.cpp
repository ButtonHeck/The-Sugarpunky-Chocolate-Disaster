#include "graphics/framebuffers/WaterReflectionFramebuffer.h"

WaterReflectionFramebuffer::WaterReflectionFramebuffer(TextureManager &textureManager)
  :
    Framebuffer(textureManager)
{}

void WaterReflectionFramebuffer::setup()
{
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager.get(TEX_FRAME_WATER_REFLECTION), 0);
  checkStatus();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
