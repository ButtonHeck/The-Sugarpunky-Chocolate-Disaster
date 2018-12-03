#ifndef SHOREFACADE_H
#define SHOREFACADE_H
#include "game/world/terrain/shore/ShoreGenerator.h"
#include "game/world/terrain/shore/ShoreShader.h"
#include "game/world/terrain/shore/ShoreRenderer.h"
#include "util/BenchmarkTimer.h"

class ShoreFacade
{
public:
  ShoreFacade(Shader &renderShader, Shader &normalsShader, const map2D_f& waterMap);
  void setup();
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);
  void draw(glm::vec3& lightDir,
            glm::mat4& lightSpaceMatrixNear,
            glm::mat4& lightSpaceMatrixFar,
            glm::mat4& projectionView,
            bool useShadows,
            bool useDebugRender);
  void drawDepthmap();
  const map2D_f& getMap() const;

private:
  ShoreShader shader;
  ShoreGenerator generator;
  ShoreRenderer renderer;
};

#endif // SHOREFACADE_H
