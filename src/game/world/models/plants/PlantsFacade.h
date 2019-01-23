#ifndef PLANTSFACADE_H
#define PLANTSFACADE_H
#include <memory>
#include "util/typeAliases.h"
#include "game/world/models/plants/LandPlantsGenerator.h"
#include "game/world/models/plants/GrassGenerator.h"
#include "game/world/models/plants/HillTreesGenerator.h"
#include "game/world/terrain/Generator.h"
#include "game/world/models/plants/PlantsShader.h"
#include "game/world/models/plants/TreesRenderer.h"
#include "game/world/models/plants/GrassRenderer.h"

class Frustum;

class PlantsFacade
{
public:
  PlantsFacade(Shader& renderPhongShader, Shader& renderGouraudShader);
  virtual ~PlantsFacade() = default;
  void setup(const map2D_f &baseMap, const map2D_f &hillMap);
  void prepareMeshesIndirectData(const glm::vec2& cameraPositionXZ, const Frustum& viewFrustum);
  void updateIndirectBufferData();
  void draw(glm::vec3 &lightDir,
            const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
            glm::mat4 &projectionView,
            glm::vec3 &viewPosition,
            bool usePhongShading,
            bool useShadows,
            bool useLandBlending);
  void drawDepthmap(bool grassCastShadow);

  //save/load routine
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);

private:
  void prepareDistributionMap(int cycles);
  void prepareMeshesIndirectData(PlantGenerator& generator, const glm::vec2& cameraPositionXZ, const Frustum& viewFrustum);
  void updateIndirectBufferData(PlantGenerator& generator);

  map2D_i distributionMap;
  PlantsShader shaders;
  LandPlantsGenerator landPlantsGenerator;
  GrassGenerator grassGenerator;
  HillTreesGenerator hillTreesGenerator;
  TreesRenderer treesRenderer;
  GrassRenderer grassRenderer;
};

#endif // PLANTSFACADE_H
