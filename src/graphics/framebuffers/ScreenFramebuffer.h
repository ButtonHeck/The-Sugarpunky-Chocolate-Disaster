#ifndef SCREENFRAMEBUFFER_H
#define SCREENFRAMEBUFFER_H
#include "graphics/openglObjects/Framebuffer.h"
#include "graphics/ScreenResolution.h"
#include "graphics/shaders/ShaderManager.h"
#include "graphics/openglObjects/BufferCollection.h"

class ScreenFramebuffer : public Framebuffer
{
public:
  ScreenFramebuffer(TextureManager& textureManager, ScreenResolution& screenResolution, ShaderManager& shaderManager);
  virtual ~ScreenFramebuffer();
  virtual void setup() override;
  void bindToViewport(int viewportWidth, int viewportHeight) = delete;
  void unbindToViewport(int viewportWidth, int viewportHeight) = delete;
  void draw(bool enableMultisampling, bool useDOF);
  void bindAppropriateFBO(bool enableMultisampling);
private:
  void setupFramebuffers();
  void setupScreenQuadBuffer();
  ScreenResolution& screenResolution;
  ShaderManager& shaderManager;
  BufferCollection screenBuffers;
  GLuint multisampleDepthRbo, multisampleFbo;
};

#endif // SCREENFRAMEBUFFER_H
