#pragma once

#include "HillsGenerator"
#include "HillsShader"
#include "HillsRenderer"

class HillsFacade
{
public:
  HillsFacade(Shader &renderShader, Shader& cullingShader, Shader &normalsShader, const map2D_f &waterMap);
  void setup();
  void createTilesAndBufferData();
  void serialize(std::ofstream &output);
  void deserialize(std::ifstream& input);
  void draw(const glm::vec3 &lightDir,
            const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
            const glm::mat4 &projectionView,
            const glm::vec3 &viewPosition,
            const glm::vec2 &viewAcceleration,
            const Frustum &viewFrustum,
            bool useFrustumCulling,
            bool useShadows,
            bool useDebugRender);
  void drawDepthmap();
  const map2D_f& getMap() const;
  const map2D_vec3& getNormalMap() const;

private:
  HillsShader shaders;
  HillsGenerator generator;
  HillsRenderer renderer;
};
