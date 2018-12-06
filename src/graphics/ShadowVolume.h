#ifndef SHADOWVOLUME_H
#define SHADOWVOLUME_H
#include "util/Settings.h"
#include "game/world/ambience/theSun/TheSunFacade.h"
#include "graphics/Frustum.h"

class ShadowVolume
{
public:
  ShadowVolume(TheSunFacade &sun);
  void update(const std::array<Frustum, NUM_SHADOW_LAYERS>& frustums);
  glm::vec3 getLightDir() const;
  const std::array<glm::mat4, NUM_SHADOW_LAYERS> &getLightSpaceMatrices() const;
private:
  struct Box
  {
    glm::vec2 ll, lr, ur, ul;
    glm::vec3 nearLL, nearLR, nearUR, nearUL;
    glm::vec3 farLL, farLR, farUR, farUL;
    glm::vec3 lightSource;
  };
  friend class ShadowVolumeRenderer;

  void updateBox(const Frustum& frustum, int layer, float sunAbsPositionY, float sunAbsPositionX);

  TheSunFacade& sun;
  glm::vec3 lightDirTo, lightDirRight, lightDirUp;
  std::array<glm::mat4, NUM_SHADOW_LAYERS> lightSpaceMatrices;
  std::array<Box, NUM_SHADOW_LAYERS> shadowBoxes;
};

#endif // SHADOWVOLUME_H
