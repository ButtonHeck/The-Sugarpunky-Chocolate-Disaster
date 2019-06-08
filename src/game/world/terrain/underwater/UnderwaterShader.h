#ifndef UNDERWATERSHADER_H
#define UNDERWATERSHADER_H
#include "Shader"

class UnderwaterShader
{
public:
  UnderwaterShader(Shader& renderShader);
  void update(const glm::vec3 &lightDir, const glm::mat4 &projectionView);

private:
  Shader& renderShader;
};

#endif // UNDERWATERSHADER_H
