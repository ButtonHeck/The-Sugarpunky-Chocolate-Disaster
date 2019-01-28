#include "game/world/models/hemisphere/HemisphereFacade.h"

HemisphereFacade::HemisphereFacade(Shader& renderShader)
  :
    hemisphere("environment/hemisphere/hemisphere.obj"),
    theSkySphere("environment/sphere/sphere.obj"),
    shader(renderShader)
{
  theSkyTransform = glm::rotate(theSkyTransform, glm::radians(94.0f), glm::vec3(1.0f, 0.0f, -0.5f));
}

void HemisphereFacade::draw(const glm::mat4 &transform,
                            const glm::mat4 &projectionView,
                            const glm::vec3 &viewPosition,
                            const glm::vec3 &lightDir)
{
  shader.update(projectionView, viewPosition, lightDir);
  shader.setTransformMatrix(transform);
  shader.setSkyMode(false);
  hemisphere.draw();
  shader.setTransformMatrix(theSkyTransform);
  shader.setSkyMode(true);
  theSkySphere.draw();
}

void HemisphereFacade::move(float timerDelta)
{
  theSkyTransform = glm::rotate(theSkyTransform, glm::radians(timerDelta), ROTATION);
}
