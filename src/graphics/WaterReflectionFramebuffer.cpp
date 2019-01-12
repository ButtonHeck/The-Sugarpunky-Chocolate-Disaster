#include "WaterReflectionFramebuffer.h"

WaterReflectionFramebuffer::WaterReflectionFramebuffer(int width, int height, TextureManager &textureManager)
  :
    width(width),
    height(height),
    textureManager(textureManager)
{}

void WaterReflectionFramebuffer::bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glViewport(0, 0, width, height);
}

void WaterReflectionFramebuffer::unbind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

WaterReflectionFramebuffer::~WaterReflectionFramebuffer()
{
  glDeleteRenderbuffers(1, &rbo);
  glDeleteFramebuffers(1, &fbo);
}

void WaterReflectionFramebuffer::setup()
{
  glCreateFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager.get(TEX_FRAME_WATER_REFLECTION), 0);
  glCreateRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    Logger::log("MS Framebuffer is not complete\n");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
