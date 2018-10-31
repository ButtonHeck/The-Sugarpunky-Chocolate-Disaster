#include "LandFacade.h"

LandFacade::LandFacade(Shader &renderShader)
  :
    shader(renderShader),
    generator(),
    renderer(generator)
{}

void LandFacade::setup(map2D_f &shoreMap)
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

void LandFacade::draw(glm::mat4& projectionView, bool useShadows, Frustum &viewFrustum, GLuint& texture)
{
  shader.update(projectionView, useShadows);
  {
    BENCHMARK("Renderer: draw flat", true);
    renderer.render(viewFrustum, texture);
  }
}

map2D_f &LandFacade::getMap()
{
  return generator.getMap();
}
