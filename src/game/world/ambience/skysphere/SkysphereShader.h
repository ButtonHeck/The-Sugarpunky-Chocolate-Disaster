#ifndef SKYSPHERESHADER_H
#define SKYSPHERESHADER_H
#include "graphics/shaders/Shader.h"

class SkysphereShader
{
public:
  SkysphereShader(Shader& renderShader);
  void update(const glm::mat4 &projectionView, const glm::vec3 &lightDir);
  void setSkysphereType(int type, const glm::mat4 &transform);

private:
  Shader& renderShader;
};

#endif // SKYSPHERESHADER_H
