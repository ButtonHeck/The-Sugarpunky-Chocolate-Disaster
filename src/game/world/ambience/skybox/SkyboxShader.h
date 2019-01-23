#ifndef SKYBOXSHADER_H
#define SKYBOXSHADER_H
#include "graphics/shaders/Shader.h"

class SkyboxShader
{
public:
  SkyboxShader(Shader& renderShader);
  void update(glm::mat4 &projectionView,
              glm::vec3 &viewPosition,
              int backgroundIndex,
              bool isStatic,
              glm::vec3 &lightDir);
private:
  Shader& renderShader;
};

#endif // SKYBOXSHADER_H
