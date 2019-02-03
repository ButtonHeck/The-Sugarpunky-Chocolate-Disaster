#ifndef THESUNSHADER_H
#define THESUNSHADER_H
#include "graphics/shaders/Shader.h"

class TheSunShader
{
public:
  TheSunShader(Shader& renderShader);
  void update(const glm::mat4 &projectionView, const glm::mat4 &model);

private:
  Shader& renderShader;
};

#endif // THESUNSHADER_H
