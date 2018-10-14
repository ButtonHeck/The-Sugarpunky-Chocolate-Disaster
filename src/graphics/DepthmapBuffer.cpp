#include "DepthmapBuffer.h"

DepthmapBuffer::~DepthmapBuffer()
{
  glDeleteFramebuffers(1, &FBO);
}

void DepthmapBuffer::setup(GLuint depthTextureUnit)
{
  glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureUnit, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "Depthmap framebuffer is not complete\n";
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthmapBuffer::bindToViewport(int frameWidth, int frameHeight)
{
  glViewport(0, 0, frameWidth, frameHeight);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void DepthmapBuffer::unbindToViewport(int frameWidth, int frameHeight)
{
  glViewport(0, 0, frameWidth, frameHeight);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
