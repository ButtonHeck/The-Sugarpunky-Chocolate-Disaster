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
  void move(float angleDegrees);

private:
  enum SKYSPHERE_TYPE : int
  {
    SKYSPHERE_AMBIENT_LIGHTING = 0,
    SKYSPHERE_STARS = 1
  };
  const glm::vec3 ROTATION = glm::normalize(glm::vec3(-0.5f, 1.0f, -0.2f));

  Skysphere theSunAmbientLightingHemisphere;
  Skysphere starsSkysphere;
  SkysphereShader shader;
  glm::mat4 starsSkysphereTransform;
};

#endif // SKYSPHEREFACADE_H
