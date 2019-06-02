#ifndef LANDFACADE_H
#define LANDFACADE_H
#include "LandGenerator.h"
#include "LandShader.h"
#include "LandRenderer.h"

class LandFacade
{
public:
  LandFacade(Shader& renderShader);
  void setup(const map2D_f& shoreMap);
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);
  void draw(const glm::vec3 &lightDir,
            const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
            const glm::mat4 &projectionView,
            bool useShadows);
  const map2D_f &getMap() const;
  void updateCellsIndirectBuffer(const Frustum& frustum);

private:
  LandShader shader;
  LandGenerator generator;
  LandRenderer renderer;
};

#endif // LANDFACADE_H
