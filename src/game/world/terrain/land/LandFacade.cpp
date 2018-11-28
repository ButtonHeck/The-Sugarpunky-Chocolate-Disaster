#include "LandFacade.h"

LandFacade::LandFacade(Shader &renderShader)
  :
    shader(renderShader),
    generator(),
    renderer(generator)
{}

void LandFacade::setup(const map2D_f &shoreMap)
{
  generator.setup(shoreMap);
}

void LandFacade::serialize(std::ofstream &output)
{
  generator.serialize(output);
}

void LandFacade::deserialize(std::ifstream &input)
{
  generator.deserialize(input);
}

void LandFacade::draw(glm::vec3 &lightDir, glm::mat4 &lightSpaceMatrixNear, glm::mat4 &lightSpaceMatrixFar, glm::mat4& projectionView, glm::vec3 &viewPosition, Frustum &viewFrustum, bool useShadows)
{
  shader.update(lightDir, lightSpaceMatrixNear, lightSpaceMatrixFar, projectionView, viewPosition, useShadows);
  renderer.render(viewFrustum);
}

const map2D_f &LandFacade::getMap() const
{
  return generator.getMap();
}
