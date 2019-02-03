#ifndef SKYBOXSHADER_H
#define SKYBOXSHADER_H
#include "graphics/shaders/Shader.h"

class SkyboxShader
{
public:
  SkyboxShader(Shader& renderShader);
  void update(const glm::mat4 &projectionView,
              const glm::vec3 &viewPosition,
              const glm::vec3 &lightDir);
  void selectSkyboxType(int type);

private:
  Shader& renderShader;
};

#endif // SKYBOXSHADER_H
