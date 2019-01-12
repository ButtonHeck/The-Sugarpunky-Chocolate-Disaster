#include "game/world/terrain/hills/HillsFacade.h"

HillsFacade::HillsFacade(Shader &renderShader, Shader &cullingShader, Shader &normalsShader, const map2D_f &waterMap)
  :
    shaders(renderShader, cullingShader, normalsShader),
    generator(shaders, waterMap),
    renderer(shaders, generator)
{}

void HillsFacade::setup()
{
  generator.setup();
}

void HillsFacade::createTilesAndBufferData()
{
  generator.createTilesAndBufferData();
}

void HillsFacade::serialize(std::ofstream &output)
{
  generator.serialize(output);
}

void HillsFacade::deserialize(std::ifstream &input)
{
  generator.deserialize(input);
}

void HillsFacade::draw(glm::vec3 &lightDir,
                       const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                       glm::mat4& projectionView,
                       glm::vec3 &viewPosition,
                       glm::vec2 &viewAcceleration,
                       Frustum &viewFrustum,
                       bool useFC,
                       bool useShadows,
                       bool useDebugRender)
{
  shaders.update(lightDir,
                 lightSpaceMatrices,
                 projectionView,
                 viewPosition,
                 viewFrustum,
                 generator.maxHeight,
                 useFC,
                 useShadows);
  shaders.debugRenderMode(false);
  renderer.render(useFC, viewAcceleration);

  if (useDebugRender)
    {
      shaders.debugRenderMode(true);
      renderer.debugRender(GL_TRIANGLES);
      shaders.updateNormals(projectionView);
      renderer.debugRender(GL_POINTS);
    }
}

void HillsFacade::drawDepthmap()
{
  renderer.renderDepthmap();
}

const map2D_f &HillsFacade::getMap() const
{
  return generator.getMap();
}
