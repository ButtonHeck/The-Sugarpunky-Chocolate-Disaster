#ifndef LANDFACADE_H
#define LANDFACADE_H
#include "game/world/terrain/land/LandGenerator.h"
#include "game/world/terrain/land/LandShader.h"
#include "game/world/terrain/land/LandRenderer.h"

class LandFacade
{
public:
  LandFacade(Shader& renderShader);
  void setup(map2D_f& shoreMap);
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);
  void draw(glm::mat4 &projectionView, bool useShadows, Frustum &viewFrustum, GLuint &texture);
  map2D_f &getMap();

private:
  LandShader shader;
  LandGenerator generator;
  LandRenderer renderer;
};

#endif // LANDFACADE_H
