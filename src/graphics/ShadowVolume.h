#ifndef SHADOWVOLUME_H
#define SHADOWVOLUME_H
#include "game/world/ambience/theSun/TheSunFacade.h"
#include "graphics/Frustum.h"

class ShadowVolume
{
public:
  constexpr static unsigned int BOX_EXPECTED_VERTICES = 4;
  constexpr static unsigned int BOX_ACTUAL_VERTICES = 8;
  constexpr static float BOX_MIN_HEIGHT = 14.0f;

  ShadowVolume(TheSunFacade &sun);
  void update(const std::array<Frustum, NUM_SHADOW_LAYERS>& frustums);
  const glm::vec3 &getLightDir() const;
  const std::array<glm::mat4, NUM_SHADOW_LAYERS> &getLightSpaceMatrices() const;

private:
  struct Box
  {
    glm::vec2 expectedLL, expectedLR, expectedUR, expectedUL;
    glm::vec3 lsPovNearLL, lsPovNearLR, lsPovNearUR, lsPovNearUL;
    glm::vec3 lsPovFarLL, lsPovFarLR, lsPovFarUR, lsPovFarUL;
    glm::vec3 lightSource;
  };
  friend class ShadowVolumeRenderer;

  void updateLightSpaceMatrix(const Frustum& frustum, int layer, float sunAbsPositionY, float sunAbsPositionX);

  TheSunFacade& sun;
  glm::vec3 lightDirTo, lightDirRight, lightDirUp;
  std::array<glm::mat4, NUM_SHADOW_LAYERS> lightSpaceMatrices;
  std::array<Box, NUM_SHADOW_LAYERS> shadowBoxes;
};

#endif // SHADOWVOLUME_H
