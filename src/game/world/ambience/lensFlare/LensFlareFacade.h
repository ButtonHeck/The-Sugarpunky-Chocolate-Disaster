#ifndef LENSFLAREFACADE_H
#define LENSFLAREFACADE_H
#include <vector>
#include "game/world/ambience/lensFlare/LensFlareElement.h"
#include "game/world/ambience/lensFlare/LensFlareRenderer.h"
#include "graphics/OpenglBuffer.h"
#include "graphics/Shader.h"
#include "graphics/TextureLoader.h"
#include "graphics/BindlessTextureManager.h"

constexpr unsigned int NUM_LENS_FLARES = 7;
constexpr float FLARES_SPACING = 0.38f;

class LensFlareFacade
{
public:
  LensFlareFacade(Shader &shader, TextureLoader& textureLoader);
  void draw(const glm::vec3 &sunWorldPosition, const glm::mat4 &projectionView, float brightnessAbs);
  void adjustFlaresPointSize(float pointSizeDivisor);
private:
  void updatePositions(glm::vec2 &sunScreenPosition, glm::vec2 &sunToCenter);
  Shader& shader;
  std::vector<LensFlareElement> flares;
  GLfloat vertices[NUM_LENS_FLARES * 3];
  OpenglBuffer basicGLBuffers;
  LensFlareRenderer renderer;
};

#endif // LENSFLAREFACADE_H
