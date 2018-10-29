#ifndef WATERFACADE_H
#define WATERFACADE_H
#include <memory>
#include "game/world/terrain/water/WaterGenerator.h"
#include "game/world/terrain/water/WaterShader.h"
#include "game/world/terrain/water/WaterRenderer.h"
#include "util/BenchmarkTimer.h"

class WaterFacade
{
public:
  WaterFacade(Shader& renderShader, Shader& cullingShader);
  void setup();
  void setupConsiderTerrain();
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);
  void draw(bool useCulling, glm::mat4 &projectionView, glm::vec3 &viewPosition, Frustum &viewFrustum);
  void bufferNewData();
  void updateAnimationFrame(double time, Options& options);
  std::vector<std::vector<float>>& getMap() const;

private:
  WaterShader shaders;
  std::shared_ptr<WaterGenerator> generator;
  WaterRenderer renderer;
};

#endif // WATERFACADE_H
