#include "ShoreFacade.h"

ShoreFacade::ShoreFacade(Shader &renderShader, Shader &normalsShader, const map2D_f &waterMap)
  :
    shader(renderShader, normalsShader),
    generator(waterMap),
    renderer(generator)
{}

void ShoreFacade::setup()
{
  generator.setup();
}

void ShoreFacade::serialize(std::ofstream &output)
{
  generator.serialize(output);
}

void ShoreFacade::deserialize(std::ifstream &input)
{
  generator.deserialize(input);
}

void ShoreFacade::draw(glm::vec3 &lightDir, glm::mat4 &lightSpaceMatrixNear, glm::mat4 &lightSpaceMatrixFar, glm::mat4 &projectionView, glm::vec3 &viewPosition, bool useShadows, bool useDebugRender)
{
  shader.update(lightDir, lightSpaceMatrixNear, lightSpaceMatrixFar, projectionView, viewPosition, useShadows);
  {
    BENCHMARK("ShoreRenderer: draw", true);
    shader.debugRenderMode(false);
    renderer.render();
  }

  if (useDebugRender)
    {
      shader.debugRenderMode(true);
      renderer.debugRender(GL_TRIANGLES);
      shader.updateNormals(projectionView);
      renderer.debugRender(GL_POINTS);
    }
}

void ShoreFacade::drawDepthmap()
{
  BENCHMARK("ShoreRenderer: draw depthmap", true);
  renderer.render();
}

const map2D_f &ShoreFacade::getMap() const
{
  return generator.getMap();
}
