#include "WaterRefractionFramebuffer"
#include "TextureManager"

WaterRefractionFramebuffer::WaterRefractionFramebuffer(TextureManager &textureManager)
  :
    Framebuffer(textureManager)
{}

void WaterRefractionFramebuffer::setup()
{
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager.get(TEX_FRAME_WATER_REFRACTION), 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureManager.get(TEX_FRAME_WATER_REFRACTION_DEPTH), 0);
  checkStatus();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
