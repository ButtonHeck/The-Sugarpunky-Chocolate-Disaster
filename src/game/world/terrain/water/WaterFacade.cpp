#include "WaterFacade"

WaterFacade::WaterFacade(Shader &renderShader, Shader &cullingShader, Shader &normalsShader)
  :
    shaders(renderShader, cullingShader, normalsShader),
    generator(shaders),
    renderer(shaders, generator)
{}

void WaterFacade::setup()
{
  generator.setup();
}

void WaterFacade::setupConsiderTerrain()
{
  generator.setupConsiderTerrain();
}

void WaterFacade::serialize(std::ofstream &output)
{
  generator.serialize(output);
}

void WaterFacade::deserialize(std::ifstream &input)
{
  generator.deserialize(input);
}

void WaterFacade::draw(const glm::vec3 &lightDir,
                       const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                       const glm::mat4& projectionView,
                       const glm::vec3 &viewPosition,
                       const Frustum &viewFrustum,
                       bool useFrustumCulling,
                       bool useDebugRender)
{
  shaders.update(lightDir,
                 lightSpaceMatrices,
                 projectionView,
                 viewPosition,
                 viewFrustum,
                 useFrustumCulling);
  shaders.debugRenderMode(false);
  renderer.render(useFrustumCulling);

  if (useDebugRender)
    {
      shaders.debugRenderMode(true);
      renderer.debugRender(GL_TRIANGLES);
      shaders.updateNormals(projectionView);
      renderer.debugRender(GL_POINTS);
    }
}

const map2D_f &WaterFacade::getMap() const
{
  return generator.getMap();
}

bool WaterFacade::hasWaterInFrame() const
{
  return renderer.anySamplesPassed();
}
