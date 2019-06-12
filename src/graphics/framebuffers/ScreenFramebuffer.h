#pragma once

#include "Framebuffer"
#include "BufferCollection"

class ShaderManager;
class ScreenResolution;

class ScreenFramebuffer : public Framebuffer
{
public:
  ScreenFramebuffer(TextureManager& textureManager, const ScreenResolution &screenResolution, ShaderManager& shaderManager);
  virtual ~ScreenFramebuffer();
  virtual void setup() override;
  void bindToViewport(int viewportWidth, int viewportHeight) = delete;
  void unbindToViewport(int viewportWidth, int viewportHeight) = delete;
  void draw(bool useMultisampling, bool useDOF);
  void bindAppropriateFBO(bool enableMultisampling) noexcept;

private:
  void setupFramebuffers();
  void setupScreenQuadBuffer();

  const ScreenResolution& screenResolution;
  ShaderManager& shaderManager;
  BufferCollection screenBuffers;
  GLuint multisampleDepthRbo, multisampleFbo;
};
