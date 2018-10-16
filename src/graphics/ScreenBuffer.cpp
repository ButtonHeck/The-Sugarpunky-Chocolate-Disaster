#include "graphics/ScreenBuffer.h"

ScreenBuffer::ScreenBuffer(ScreenResolution &screenResolution, TextureManager &textureManager, ShaderManager &shaderManager)
  :
    screenResolution(screenResolution),
    textureManager(textureManager),
    shaderManager(shaderManager),
    screenBuffers(VAO | VBO)
{}

ScreenBuffer::~ScreenBuffer()
{
  glDeleteFramebuffers(1, &multisampleFBO);
  glDeleteFramebuffers(1, &screenFBO);
  glDeleteRenderbuffers(1, &screenDepthRBO);
  glDeleteRenderbuffers(1, &multisampleDepthRBO);
}

void ScreenBuffer::setup()
{
  setupFramebuffers();
  setupScreenQuadBuffer();
}

void ScreenBuffer::setupFramebuffers()
{
  //multisample
  glGenFramebuffers(1, &multisampleFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, multisampleFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureManager.get(TEX_FRAME_MULTISAMPLED), 0);
  glGenRenderbuffers(1, &multisampleDepthRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, multisampleDepthRBO);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, MULTISAMPLES, GL_DEPTH24_STENCIL8, screenResolution.getWidth(), screenResolution.getHeight());
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, multisampleDepthRBO);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE)
    std::cout << "MS Framebuffer is not complete\n";
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //intermediate FBO (or direct off-screen FBO without multisampling)
  glGenFramebuffers(1, &screenFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         HDR_ENABLED ? textureManager.get(TEX_FRAME_HDR) : textureManager.get(TEX_FRAME), 0);
  //we don't need depth data if we use this FBO as intermediate, but we DO need it if theres no multisampling
  glGenRenderbuffers(1, &screenDepthRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, screenDepthRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenResolution.getWidth(), screenResolution.getHeight());
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, screenDepthRBO);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "Intermediate Framebuffer is not complete\n";
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ScreenBuffer::setupScreenQuadBuffer()
{
  screenBuffers.bind(VAO | VBO);
  constexpr float SCREEN_VERTICES[] = {
    -1.0f, -1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 1.0f, 1.0f,
     1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(SCREEN_VERTICES), &SCREEN_VERTICES, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ScreenBuffer::draw(bool enableMultisampling)
{
  if (enableMultisampling)
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampleFBO);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, screenFBO);
      glBlitFramebuffer(0, 0, screenResolution.getWidth(), screenResolution.getHeight(), 0, 0, screenResolution.getWidth(), screenResolution.getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
  else
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, screenFBO);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }
  shaderManager.get(SHADER_MS_TO_DEFAULT).use();
  screenBuffers.bind(VAO);
  glDisable(GL_DEPTH_TEST);
  glDrawArrays(GL_TRIANGLES, 0, VERTICES_PER_TILE);
  glEnable(GL_DEPTH_TEST);
}

void ScreenBuffer::bindAppropriateFBO(bool enableMultisampling)
{
  glBindFramebuffer(GL_FRAMEBUFFER, enableMultisampling ? multisampleFBO : screenFBO);
}
