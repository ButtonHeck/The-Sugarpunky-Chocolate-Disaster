#ifndef LENSFLARESHADER_H
#define LENSFLARESHADER_H
#include "graphics/shaders/Shader.h"

class LensFlareShader
{
public:
  LensFlareShader(Shader& renderShader);
  void update(float brightnessFlares, float brightnessHalo);

private:
  Shader& renderShader;
};

#endif // LENSFLARESHADER_H
