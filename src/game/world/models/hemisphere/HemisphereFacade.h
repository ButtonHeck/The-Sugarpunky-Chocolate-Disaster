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
private:
  Hemisphere hemisphere;
  HemisphereShader shader;
};

#endif // HEMISPHEREFACADE_H
