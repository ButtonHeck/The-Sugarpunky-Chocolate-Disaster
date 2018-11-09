#ifndef HILLSFACADE_H
#define HILLSFACADE_H
#include <memory>
#include "game/world/terrain/hills/HillsGenerator.h"
#include "game/world/terrain/hills/HillsShader.h"
#include "game/world/terrain/hills/HillsRenderer.h"
#include "util/BenchmarkTimer.h"

class HillsFacade
{
public:
  HillsFacade(Shader &renderShader, Shader& cullingShader, Shader &normalsShader, const map2D_f &waterMap);
  void setup();
  void createTilesAndBufferData();
  void serialize(std::ofstream &output);
  void deserialize(std::ifstream& input);
  void draw(bool useFC,
            bool useShadows,
            bool useDebugRender,
            glm::mat4& projectionView,
            glm::vec3 &viewPosition,
            Frustum &viewFrustum);
  void drawDepthmap();
  const map2D_f& getMap() const;

private:
  HillsShader shaders;
  HillsGenerator generator;
  HillsRenderer renderer;
};

#endif // HILLSFACADE_H
