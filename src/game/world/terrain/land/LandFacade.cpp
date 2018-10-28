#include "LandFacade.h"

LandFacade::LandFacade(Shader &renderShader)
  :
    shader(renderShader),
    generator(std::make_shared<LandGenerator>()),
    renderer(generator)
{}

void LandFacade::setup(std::vector<std::vector<float> > &shoreMap)
{
  generator->setup(shoreMap);
}

void LandFacade::serialize(std::ofstream &output)
{
  generator->serialize(output);
}

void LandFacade::deserialize(std::ifstream &input)
{
  generator->deserialize(input);
}

void LandFacade::draw(glm::mat4& projectionView, bool useShadows, Frustum &viewFrustum, GLuint& texture)
{
  shader.update(projectionView, useShadows);
  {
    BENCHMARK("Renderer: draw flat", true);
    renderer.render(viewFrustum, texture);
  }
}

std::vector<std::vector<float> > &LandFacade::getMap() const
{
  return generator->getMap();
}
