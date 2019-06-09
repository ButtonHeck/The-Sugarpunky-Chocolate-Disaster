#include "WaterReflectionFramebuffer"
#include "GraphicsSettings"
#include "TextureManager"

WaterReflectionFramebuffer::WaterReflectionFramebuffer(TextureManager &textureManager)
  :
    Framebuffer(textureManager)
{
  glCreateRenderbuffers(1, &rbo);
}

WaterReflectionFramebuffer::~WaterReflectionFramebuffer()
{
  glDeleteRenderbuffers(1, &rbo);
}

void WaterReflectionFramebuffer::setup()
{
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager.get(TEX_FRAME_WATER_REFLECTION), 0);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, FRAME_WATER_REFLECTION_WIDTH, FRAME_WATER_REFLECTION_HEIGHT);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
  checkStatus();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
