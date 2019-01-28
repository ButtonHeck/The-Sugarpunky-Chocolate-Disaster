#ifndef HEMISPHERESHADER_H
#define HEMISPHERESHADER_H
#include "graphics/shaders/Shader.h"

class HemisphereShader
{
public:
  HemisphereShader(Shader& renderShader);
  void update(const glm::mat4 &projectionView,
              const glm::vec3 &viewPosition,
              const glm::vec3 &lightDir);
  void setTransformMatrix(const glm::mat4 &transform);
  void setSkyMode(bool drawSky);
private:
  Shader& renderShader;
};

#endif // HEMISPHERESHADER_H
