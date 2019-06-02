#ifndef SHADOWVOLUME_H
#define SHADOWVOLUME_H
#include <array>
#include "../../src/game/world/ambience/theSun/TheSunFacade.h"
#include "../../src/graphics/Frustum.h"

class ShadowVolume
{
public:
  constexpr static unsigned int BOX_EXPECTED_VERTICES = 4;
  constexpr static unsigned int BOX_ACTUAL_VERTICES = 8;
  constexpr static float BOX_MIN_HEIGHT = 14.0f;

  ShadowVolume() = default;
  void update(const std::array<Frustum, NUM_SHADOW_LAYERS>& frustums, const TheSunFacade &theSunFacade);
  const std::array<glm::mat4, NUM_SHADOW_LAYERS> &getLightSpaceMatrices() const;

private:
  constexpr static float SHADOW_BOX_MAX_OFFSET_X = 30.0f;
  constexpr static float SHADOW_BOX_MAP_BORDER_OFFSET = 0.5f;
  const float SHADOW_BOXES_MAX_HEIGHT[NUM_SHADOW_LAYERS] = {BOX_MIN_HEIGHT, BOX_MIN_HEIGHT * 2, BOX_MIN_HEIGHT * 4};

  struct Box
  {
    glm::vec2 expectedLL, expectedLR, expectedUR, expectedUL;
    //lsPov - light source point of view
    glm::vec3 lsPovNearLL, lsPovNearLR, lsPovNearUR, lsPovNearUL;
    glm::vec3 lsPovFarLL, lsPovFarLR, lsPovFarUR, lsPovFarUL;
    glm::vec3 localLightSource;
  };
  friend class ShadowVolumeRenderer;

  void updateLightSpaceMatrix(const Frustum& frustum, int layer, float sunAbsPositionY, float sunAbsPositionX);

  glm::vec3 lightDirTo, lightDirRight, lightDirUp;
  std::array<glm::mat4, 3> lightSpaceMatrices;
  std::array<Box, NUM_SHADOW_LAYERS> shadowBoxes;
};

#endif // SHADOWVOLUME_H
