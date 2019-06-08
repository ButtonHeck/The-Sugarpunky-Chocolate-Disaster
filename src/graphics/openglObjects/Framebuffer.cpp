#include "Framebuffer"

Framebuffer::Framebuffer(TextureManager &textureManager)
  :
    textureManager(textureManager)
{
  glCreateFramebuffers(1, &fbo);
}

Framebuffer::~Framebuffer()
{
  glDeleteFramebuffers(1, &fbo);
}

void Framebuffer::checkStatus()
{
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    Logger::log("Framebuffer is not complete\n");
}

void Framebuffer::bindToViewport(int viewportWidth, int viewportHeight)
{
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glViewport(0, 0, viewportWidth, viewportHeight);
}

void Framebuffer::unbindToViewport(int viewportWidth, int viewportHeight)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, viewportWidth, viewportHeight);
}
