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
                       glm::mat4 &lightSpaceMatrixNear,
                       glm::mat4 &lightSpaceMatrixFar,
                       glm::mat4& projectionView,
                       glm::vec3 &viewPosition,
                       Frustum &viewFrustum,
                       bool useFC,
                       bool useShadows,
                       bool useDebugRender)
{
  shaders.update(lightDir, lightSpaceMatrixNear, lightSpaceMatrixFar, projectionView, viewPosition, viewFrustum, generator.maxHeight, useFC, useShadows);
  shaders.debugRenderMode(false);
  renderer.render(useFC);

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
