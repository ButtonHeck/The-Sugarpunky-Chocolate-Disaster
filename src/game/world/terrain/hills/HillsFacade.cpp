#include "HillsFacade.h"

HillsFacade::HillsFacade(Shader &renderShader,
                         Shader &cullingShader,
                         Shader &normalsShader,
                         const map2D_f &waterMap)
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
  generator.serialize(output, true, 4);
}

void HillsFacade::deserialize(std::ifstream &input)
{
  generator.deserialize(input);
}

void HillsFacade::draw(const glm::vec3 &lightDir,
                       const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                       const glm::mat4& projectionView,
                       const glm::vec3 &viewPosition,
                       const glm::vec2 &viewAcceleration,
                       const Frustum &viewFrustum,
                       bool useFrustumCulling,
                       bool useShadows,
                       bool useDebugRender)
{
  shaders.update(lightDir,
                 lightSpaceMatrices,
                 projectionView,
                 viewPosition,
                 viewFrustum,
                 generator.maxHeight,
                 useFrustumCulling,
                 useShadows);
  shaders.debugRenderMode(false);
  renderer.render(useFrustumCulling, viewAcceleration);

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

const map2D_vec3 &HillsFacade::getNormalMap() const
{
  return generator.normalMap;
}
