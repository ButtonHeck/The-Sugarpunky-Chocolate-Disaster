#ifndef HEMISPHEREFACADE_H
#define HEMISPHEREFACADE_H
#include "game/world/models/hemisphere/Hemisphere.h"
#include "game/world/models/hemisphere/HemisphereShader.h"

class HemisphereFacade
{
public:
  HemisphereFacade(Shader& renderShader);
  void draw(const glm::mat4 &transform,
            const glm::mat4 &projectionView,
            const glm::vec3 &viewPosition,
            const glm::vec3 &lightDir);
  void move(float timerDelta);
private:
  const glm::vec3 ROTATION = glm::normalize(glm::vec3(-0.5f, 1.0f, -0.2f));
  Hemisphere hemisphere;
  Hemisphere theSkySphere;
  HemisphereShader shader;
  glm::mat4 theSkyTransform;
};

#endif // HEMISPHEREFACADE_H
