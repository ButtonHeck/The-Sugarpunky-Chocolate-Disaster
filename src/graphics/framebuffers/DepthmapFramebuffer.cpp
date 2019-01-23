#include "graphics/framebuffers/DepthmapFramebuffer.h"

DepthmapFramebuffer::DepthmapFramebuffer(TextureManager &textureManager)
  :
    Framebuffer(textureManager)
{}

void DepthmapFramebuffer::setup()
{
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, textureManager.get(TEX_DEPTH_MAP_SUN), 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  checkStatus();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
