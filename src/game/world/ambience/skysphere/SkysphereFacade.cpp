#include "game/world/ambience/skysphere/SkysphereFacade.h"

SkysphereFacade::SkysphereFacade(Shader& renderShader)
  :
    theSunAmbientLightingHemisphere("environment/hemisphere/hemisphere.obj"),
    starsSkysphere("environment/sphere/sphere.obj", glm::rotate(glm::mat4(), glm::radians(94.0f), glm::vec3(1.0f, 0.0f, -0.5f))),
    shader(renderShader)
{}

void SkysphereFacade::draw(const glm::mat4 &transform,
                           const glm::mat4 &projectionView,
                           const glm::vec3 &lightDir)
{
  shader.update(projectionView, lightDir);
  shader.setSkysphereType(SKYSPHERE_AMBIENT_LIGHTING, transform);
  theSunAmbientLightingHemisphere.draw();
  shader.setSkysphereType(SKYSPHERE_STARS, starsSkysphere.getRotationTransform());
  starsSkysphere.draw();
}

void SkysphereFacade::moveStarsSkysphere(float angleDegrees)
{
  starsSkysphere.rotate(angleDegrees, SKYSPHERE_STARS_ROTATION);
}
