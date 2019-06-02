#ifndef WATERFACADE_H
#define WATERFACADE_H
#include "WaterGenerator.h"
#include "WaterShader.h"
#include "WaterRenderer.h"
#include "../../src/util/BenchmarkTimer.h"

class WaterFacade
{
public:
  WaterFacade(Shader& renderShader, Shader& cullingShader, Shader &normalsShader);
  void setup();
  void setupConsiderTerrain();
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);
  void draw(const glm::vec3 &lightDir,
            const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
            const glm::mat4 &projectionView,
            const glm::vec3 &viewPosition,
            const Frustum &viewFrustum,
            bool useFrustumCulling,
            bool useDebugRender);
  const map2D_f& getMap() const;
  bool hasWaterInFrame() const;

private:
  WaterShader shaders;
  WaterGenerator generator;
  WaterRenderer renderer;
};

#endif // WATERFACADE_H
