#include "game/world/models/hemisphere/HemisphereFacade.h"

HemisphereFacade::HemisphereFacade(Shader& renderShader)
  :
    hemisphere("hemisphere/hemisphere.obj"),
    shader(renderShader)
{}

void HemisphereFacade::draw(const glm::mat4 &transform,
                            const glm::mat4 &projectionView,
                            const glm::vec3 &viewPosition,
                            const glm::vec3 &lightDir)
{
  shader.update(transform, projectionView, viewPosition, lightDir);
  hemisphere.draw();
}
