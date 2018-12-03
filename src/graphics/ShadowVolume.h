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
  void update(Frustum &nearFrustum, Frustum& middleFrustum, Frustum& farFrustum);
  glm::vec3 getLightDir() const;
  glm::mat4 getLightSpaceMatrixNear() const;
  glm::mat4 getLightSpaceMatrixMiddle() const;
  glm::mat4 getLightSpaceMatrixFar() const;
private:
  struct Box
  {
    glm::vec2 ll, lr, ur, ul;
    glm::vec3 nearLL, nearLR, nearUR, nearUL;
    glm::vec3 farLL, farLR, farUR, farUL;
    glm::vec3 lightSource;
  };
  friend class ShadowVolumeRenderer;

  TheSunFacade& sun;
  glm::vec3 lightDirTo;
  glm::mat4 lightSpaceMatrixNear;
  glm::mat4 lightSpaceMatrixMiddle;
  glm::mat4 lightSpaceMatrixFar;
  Box nearBox;
  Box middleBox;
  Box farBox;
};

#endif // SHADOWVOLUME_H
