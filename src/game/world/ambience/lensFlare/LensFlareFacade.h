#ifndef LENSFLAREFACADE_H
#define LENSFLAREFACADE_H
#include <vector>
#include "game/world/ambience/lensFlare/LensFlareElement.h"
#include "game/world/ambience/lensFlare/LensFlareRenderer.h"
#include "graphics/TextureUnits.h"
#include "graphics/OpenglBuffer.h"
#include "graphics/Shader.h"

constexpr unsigned int NUM_LENS_FLARES = 7;

class LensFlareFacade
{
public:
  LensFlareFacade(Shader &shader, float spacing);
  void draw(const glm::vec3 &sunWorldPosition, const glm::mat4 &projectionView, float brightnessAbs);
private:
  void updatePositions(glm::vec2 &sunScreenPosition, glm::vec2 &sunToCenter);
  Shader& shader;
  float spacing;
  std::vector<LensFlareElement> flares;
  GLfloat vertices[NUM_LENS_FLARES * 3];
  OpenglBuffer basicGLBuffers;
  LensFlareRenderer renderer;
};

#endif // LENSFLAREFACADE_H
