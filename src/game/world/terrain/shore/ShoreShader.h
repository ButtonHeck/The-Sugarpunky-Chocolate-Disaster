#ifndef SHORESHADER_H
#define SHORESHADER_H
#include "graphics/Shader.h"

class ShoreShader
{
public:
  ShoreShader(Shader& renderShader);
  void update(glm::mat4 &projectionView, bool useShadows);
private:
  Shader& renderShader;
};

#endif // SHORESHADER_H
