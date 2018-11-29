#ifndef SHADOWVOLUME_H
#define SHADOWVOLUME_H
#include <algorithm>
#include "util/Settings.h"
#include "game/world/ambience/theSun/TheSunFacade.h"
#include "graphics/Frustum.h"

class ShadowVolume
{
public:
  ShadowVolume(TheSunFacade &sun);
  void update(Frustum &nearFrustum, Frustum& farFrustum, float aspect);
  glm::vec3 getLightDir() const;
  glm::mat4 getLightSpaceMatrixNear() const;
  glm::mat4 getLightSpaceMatrixFar() const;
private:
  struct Box
  {
    glm::vec2 ll, lr, ur, ul;
  };
  friend class ShadowVolumeRenderer;

  TheSunFacade& sun;
  glm::vec3 lightDirTo;
  glm::mat4 lightSpaceMatrixNear;
  glm::mat4 lightSpaceMatrixFar;
  Box nearBox;
  Box farBox;
};

#endif // SHADOWVOLUME_H
