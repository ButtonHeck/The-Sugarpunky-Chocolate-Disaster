#ifndef PLANTSSHADER_H
#define PLANTSSHADER_H
#include "graphics/Shader.h"

class PlantsShader
{
public:
  PlantsShader(Shader& renderPhongShader, Shader& renderGouraudShader, Shader& shadowShader);
  void update(glm::mat4& projectionView,
              glm::vec3& viewPosition,
              bool shadowOnTrees,
              bool useShadows,
              bool useFlatBlending);
private:
  friend class PlantsFacade;
  Shader& renderPhongShader;
  Shader& renderGouraudShader;
  Shader& shadowShader;
};

#endif // PLANTSSHADER_H
