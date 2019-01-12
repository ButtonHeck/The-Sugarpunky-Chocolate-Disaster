#include "WaterRefractionFramebuffer.h"

WaterRefractionFramebuffer::WaterRefractionFramebuffer(int width, int height, TextureManager &textureManager)
  :
    width(width),
    height(height),
    textureManager(textureManager)
{}

void WaterRefractionFramebuffer::bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glViewport(0, 0, width, height);
}

void WaterRefractionFramebuffer::unbind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

WaterRefractionFramebuffer::~WaterRefractionFramebuffer()
{
  glDeleteFramebuffers(1, &fbo);
}

void WaterRefractionFramebuffer::setup()
{
  glCreateFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager.get(TEX_FRAME_WATER_REFRACTION), 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureManager.get(TEX_FRAME_WATER_REFRACTION_DEPTH), 0);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    Logger::log("MS Framebuffer is not complete\n");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
