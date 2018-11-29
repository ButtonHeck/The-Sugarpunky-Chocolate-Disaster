#ifndef SHADOWVOLUME_H
#define SHADOWVOLUME_H
#include <algorithm>
#include "util/Settings.h"
#include "game/world/ambience/theSun/TheSunFacade.h"
#include "game/Camera.h"
#include "graphics/Frustum.h"

class ShadowVolume
{
public:
  ShadowVolume(TheSunFacade &sun);
  void update(Camera &camera, Frustum &nearFrustum, Frustum& farFrustum, float aspect);
  glm::vec3 getLightDir() const;
  glm::mat4 getLightSpaceMatrixNear() const;
  glm::mat4 getLightSpaceMatrixFar() const;
private:
  TheSunFacade& sun;
  float nearPlane = 0.1f;
  float farPlane = static_cast<float>(WORLD_WIDTH);
  const float LEFT_PLANE = -HALF_WORLD_HEIGHT_F;
  const float RIGHT_PLANE = HALF_WORLD_HEIGHT_F;
  float topPlane = HALF_WORLD_HEIGHT_F;
  float bottomPlane = -HALF_WORLD_HEIGHT_F;

  glm::vec3 lightDirTo;
  glm::mat4 lightSpaceMatrixNear;
  glm::mat4 lightSpaceMatrixFar;
};

#endif // SHADOWVOLUME_H
