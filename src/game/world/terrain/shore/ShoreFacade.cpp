#include "ShoreFacade.h"

ShoreFacade::ShoreFacade(Shader &renderShader, std::vector<std::vector<float>>& waterMap)
  :
    shader(renderShader),
    generator(std::make_shared<ShoreGenerator>(waterMap)),
    renderer(generator)
{}

void ShoreFacade::setup()
{
  generator->setup();
}

void ShoreFacade::serialize(std::ofstream &output)
{
  generator->serialize(output);
}

void ShoreFacade::deserialize(std::ifstream &input)
{
  generator->deserialize(input);
}

void ShoreFacade::draw(glm::mat4 &projectionView, bool useShadows)
{
  shader.update(projectionView, useShadows);
  {
    BENCHMARK("Renderer: draw shore", true);
    renderer.render();
  }
}

void ShoreFacade::drawDepthmap()
{
  renderer.render();
}

std::vector<std::vector<float> > &ShoreFacade::getMap() const
{
  return generator->getMap();
}
