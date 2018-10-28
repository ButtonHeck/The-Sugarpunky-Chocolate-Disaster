#ifndef LANDSHADER_H
#define LANDSHADER_H
#include "graphics/Shader.h"

class LandShader
{
public:
  LandShader(Shader& renderShader);
  void update(glm::mat4& projectionView, bool useShadows);
private:
  Shader& renderShader;
};

#endif // LANDSHADER_H
