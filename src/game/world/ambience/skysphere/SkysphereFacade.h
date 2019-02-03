#ifndef SKYSPHEREFACADE_H
#define SKYSPHEREFACADE_H
#include "game/world/ambience/skysphere/Skysphere.h"
#include "game/world/ambience/skysphere/SkysphereShader.h"

class SkysphereFacade
{
public:
  SkysphereFacade(Shader& renderShader);
  void draw(const glm::mat4 &transform,
            const glm::mat4 &projectionView,
            const glm::vec3 &lightDir);
  void moveStarsSkysphere(float angleDegrees);

private:
  enum SKYSPHERE_TYPE : int
  {
    SKYSPHERE_AMBIENT_LIGHTING = 0,
    SKYSPHERE_STARS = 1
  };
  const glm::vec3 SKYSPHERE_STARS_ROTATION = glm::normalize(glm::vec3(-0.5f, 1.0f, -0.2f));

  Skysphere theSunAmbientLightingHemisphere, starsSkysphere;
  SkysphereShader shader;
};

#endif // SKYSPHEREFACADE_H
