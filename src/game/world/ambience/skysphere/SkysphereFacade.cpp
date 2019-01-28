#include "game/world/ambience/skysphere/SkysphereFacade.h"

SkysphereFacade::SkysphereFacade(Shader& renderShader)
  :
    theSunAmbientLightingHemisphere("environment/hemisphere/hemisphere.obj"),
    starsSkysphere("environment/sphere/sphere.obj"),
    shader(renderShader)
{
  //initial arbitrary rotation transformation
  starsSkysphereTransform = glm::rotate(starsSkysphereTransform, glm::radians(94.0f), glm::vec3(1.0f, 0.0f, -0.5f));
}

void SkysphereFacade::draw(const glm::mat4 &transform,
                           const glm::mat4 &projectionView,
                           const glm::vec3 &lightDir)
{
  shader.update(projectionView, lightDir);
  shader.setTransformMatrix(transform);
  shader.setSkysphereType(SKYSPHERE_AMBIENT_LIGHTING);
  theSunAmbientLightingHemisphere.draw();
  shader.setTransformMatrix(starsSkysphereTransform);
  shader.setSkysphereType(SKYSPHERE_STARS);
  starsSkysphere.draw();
}

void SkysphereFacade::move(float angleDegrees)
{
  starsSkysphereTransform = glm::rotate(starsSkysphereTransform, glm::radians(angleDegrees), ROTATION);
}
