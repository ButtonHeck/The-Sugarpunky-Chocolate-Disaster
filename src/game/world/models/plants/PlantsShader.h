#ifndef PLANTSSHADER_H
#define PLANTSSHADER_H
#include "graphics/shaders/Shader.h"

class PlantsShader
{
public:
  PlantsShader(Shader& renderPhongShader, Shader& renderGouraudShader);
  void updateAllPlants(bool usePhongShading,
                       const glm::vec3 &lightDir,
                       const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                       const glm::mat4 &projectionView,
                       const glm::vec3 &viewPosition,
                       bool useShadows,
                       bool useLandBlending);
  void updateGrass();
  void switchToGrass(bool isGrass);
  void switchToLowPoly(bool isLowPoly);

private:
  friend class PlantsFacade;
  Shader& renderPhongShader;
  Shader& renderGouraudShader;
  Shader* currentShader;
};

#endif // PLANTSSHADER_H
