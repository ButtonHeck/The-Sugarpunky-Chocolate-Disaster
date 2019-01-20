#include "WaterReflectionFramebuffer.h"

WaterReflectionFramebuffer::WaterReflectionFramebuffer(TextureManager &textureManager)
  :
    textureManager(textureManager)
{}

void WaterReflectionFramebuffer::bindToViewport(int viewportWidth, int viewportHeight)
{
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glViewport(0, 0, viewportWidth, viewportHeight);
}

void WaterReflectionFramebuffer::unbindToViewport(int viewportWidth, int viewportHeight)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, viewportWidth, viewportHeight);
}

WaterReflectionFramebuffer::~WaterReflectionFramebuffer()
{
  glDeleteFramebuffers(1, &fbo);
}

void WaterReflectionFramebuffer::setup()
{
  glCreateFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager.get(TEX_FRAME_WATER_REFLECTION), 0);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    Logger::log("MS Framebuffer is not complete\n");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
