#ifndef UNDERWATERSHADER_H
#define UNDERWATERSHADER_H
#include "graphics/shaders/Shader.h"

class UnderwaterShader
{
public:
  UnderwaterShader(Shader& renderShader);
  void update(glm::vec3& lightDir, glm::mat4& projectionView);
private:
  Shader& renderShader;
};

#endif // UNDERWATERSHADER_H
