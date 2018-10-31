#include "game/world/terrain/hills/HillsFacade.h"

HillsFacade::HillsFacade(Shader &renderShader, Shader &cullingShader, map2D_f &waterMap)
  :
    shaders(renderShader, cullingShader),
    generator(std::make_shared<HillsGenerator>(shaders, waterMap)),
    renderer(shaders, generator)
{}

void HillsFacade::setup()
{
  generator->setup();
}

void HillsFacade::createTilesAndBufferData()
{
  generator->createTilesAndBufferData();
}

void HillsFacade::serialize(std::ofstream &output)
{
  generator->serialize(output);
}

void HillsFacade::deserialize(std::ifstream &input)
{
  generator->deserialize(input);
}

void HillsFacade::draw(bool useFC, bool useShadows, glm::mat4& projectionView, glm::vec3 &viewPosition, Frustum &viewFrustum)
{
  shaders.update(useFC, useShadows, projectionView, viewPosition, viewFrustum, generator->maxHeight);
  {
    BENCHMARK("Renderer: draw hills", true);
    renderer.render(useFC);
  }
}

void HillsFacade::drawDepthmap()
{
  renderer.renderDepthmap();
}

map2D_f &HillsFacade::getMap() const
{
  return generator->getMap();
}
