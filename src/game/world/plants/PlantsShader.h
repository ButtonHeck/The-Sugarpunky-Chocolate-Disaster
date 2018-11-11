#ifndef PLANTSSHADER_H
#define PLANTSSHADER_H
#include "graphics/Shader.h"

class PlantsShader
{
public:
  PlantsShader(Shader& renderPhongShader, Shader& renderGouraudShader);
  void updateAllPlants(glm::mat4& projectionView,
                    glm::vec3& viewPosition,
                    bool usePhongShading,
                    bool shadowOnTrees,
                    bool useShadows,
                    bool useFlatBlending);
  void updateGrass(bool usePhongShading);
  void switchToGrass(bool usePhongShading, bool isGrass);
private:
  friend class PlantsFacade;
  Shader& renderPhongShader;
  Shader& renderGouraudShader;
};

#endif // PLANTSSHADER_H
