#ifndef SCREENBUFFER_H
#define SCREENBUFFER_H
#include <GL/glew.h>
#include "graphics/TextureManager.h"
#include "game/ScreenResolution.h"
#include "graphics/ShaderManager.h"

class ScreenBuffer
{
public:
  ScreenBuffer(ScreenResolution& screenResolution, TextureManager& textureManager, ShaderManager& shaderManager);
  ~ScreenBuffer();
  void setupBuffer();
  void drawBuffer(bool enableMultisampling);
  void bindAppropriateFBO(bool enableMultisampling);
private:
  void setupFramebuffers();
  void setupScreenQuadBuffer();
  ScreenResolution& screenResolution;
  TextureManager& textureManager;
  ShaderManager& shaderManager;
  GLuint screenVAO, screenVBO, screenDepthRBO, screenFBO;
  GLuint multisampleDepthRBO, multisampleFBO;
};

#endif // SCREENBUFFER_H
