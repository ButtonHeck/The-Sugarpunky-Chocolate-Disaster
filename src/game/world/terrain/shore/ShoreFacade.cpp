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

void ShoreFacade::draw(glm::vec3 &lightDir,
                       const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                       glm::mat4 &projectionView,
                       bool useShadows,
                       bool useDebugRender,
                       bool useClipDistance)
{
  shader.update(lightDir,
                lightSpaceMatrices,
                projectionView, useShadows, useClipDistance);
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
