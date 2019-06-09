#include "ScreenFramebuffer"
#include "TextureManager"
#include "BenchmarkTimer"
#include "ShaderManager"
#include "Shader"
#include "ScreenResolution"

ScreenFramebuffer::ScreenFramebuffer(TextureManager &textureManager, const ScreenResolution &screenResolution, ShaderManager &shaderManager)
  :
    Framebuffer(textureManager),
    screenResolution(screenResolution),
    shaderManager(shaderManager),
    screenBuffers(VAO | VBO)
{
  glCreateFramebuffers(1, &multisampleFbo);
  glCreateRenderbuffers(1, &multisampleDepthRbo);
}

ScreenFramebuffer::~ScreenFramebuffer()
{
  glDeleteFramebuffers(1, &multisampleFbo);
  glDeleteRenderbuffers(1, &multisampleDepthRbo);
}

void ScreenFramebuffer::setup()
{
  BENCHMARK("ScreenBuffer: setup", false);
  setupFramebuffers();
  setupScreenQuadBuffer();
}

void ScreenFramebuffer::setupFramebuffers()
{
  //multisample
  glBindFramebuffer(GL_FRAMEBUFFER, multisampleFbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureManager.get(TEX_FRAME_MULTISAMPLED), 0);
  glBindRenderbuffer(GL_RENDERBUFFER, multisampleDepthRbo);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, MULTISAMPLES, GL_DEPTH_COMPONENT24, screenResolution.getWidth(), screenResolution.getHeight());
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, multisampleDepthRbo);
  checkStatus();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //intermediate FBO (or direct off-screen FBO without multisampling)
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         HDR_ENABLED ? textureManager.get(TEX_FRAME_HDR) : textureManager.get(TEX_FRAME), 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureManager.get(TEX_FRAME_DEPTH), 0);
  checkStatus();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ScreenFramebuffer::setupScreenQuadBuffer()
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
  BufferCollection::bindZero(VAO | VBO);
}

void ScreenFramebuffer::draw(bool useMultisampling, bool useDOF)
{
  BENCHMARK("ScreenBuffer: draw", true);
  if (useMultisampling)
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampleFbo);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
      glBlitFramebuffer(0, 0, screenResolution.getWidth(), screenResolution.getHeight(),
                        0, 0, screenResolution.getWidth(), screenResolution.getHeight(),
                        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
  else
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }
  shaderManager.get(SHADER_MS_TO_DEFAULT).use();
  shaderManager.get(SHADER_MS_TO_DEFAULT).setBool("u_useDOF", useDOF);
  screenBuffers.bind(VAO);
  glDisable(GL_DEPTH_TEST);
  glDrawArrays(GL_TRIANGLES, 0, VERTICES_PER_QUAD);
  glEnable(GL_DEPTH_TEST);
}

void ScreenFramebuffer::bindAppropriateFBO(bool enableMultisampling)
{
  glBindFramebuffer(GL_FRAMEBUFFER, enableMultisampling ? multisampleFbo : fbo);
}
