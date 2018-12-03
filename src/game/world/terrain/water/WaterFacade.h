#ifndef WATERFACADE_H
#define WATERFACADE_H
#include "game/world/terrain/water/WaterGenerator.h"
#include "game/world/terrain/water/WaterShader.h"
#include "game/world/terrain/water/WaterRenderer.h"
#include "util/BenchmarkTimer.h"

class WaterFacade
{
public:
  WaterFacade(Shader& renderShader, Shader& cullingShader, Shader &normalsShader);
  void setup();
  void setupConsiderTerrain();
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);
  void draw(glm::vec3 &lightDir,
            glm::mat4 &lightSpaceMatrixNear,
            glm::mat4 &lightSpaceMatrixMiddle,
            glm::mat4 &lightSpaceMatrixFar,
            glm::mat4 &projectionView,
            glm::vec3 &viewPosition,
            Frustum &viewFrustum,
            bool useCulling,
            bool useDebugRender);
  void bufferNewData();
  void updateAnimationFrame(double time, Options& options);
  const map2D_f& getMap() const;

private:
  WaterShader shaders;
  WaterGenerator generator;
  WaterRenderer renderer;
};

#endif // WATERFACADE_H
