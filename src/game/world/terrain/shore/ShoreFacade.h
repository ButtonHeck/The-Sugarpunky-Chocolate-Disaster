#ifndef SHOREFACADE_H
#define SHOREFACADE_H
#include "ShoreGenerator.h"
#include "ShoreShader.h"
#include "ShoreRenderer.h"
#include "../../src/util/BenchmarkTimer.h"

class ShoreFacade
{
public:
  ShoreFacade(Shader &renderShader, Shader &normalsShader, const map2D_f& waterMap);
  void setup();
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);
  void draw(const glm::vec3 &lightDir,
            const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
            const glm::mat4 &projectionView,
            bool useShadows,
            bool useDebugRender,
            bool useClipDistanceReflection,
            bool useClipDistanceRefraction);
  void drawDepthmap();
  const map2D_f& getMap() const;

private:
  ShoreShader shader;
  ShoreGenerator generator;
  ShoreRenderer renderer;
};

#endif // SHOREFACADE_H
