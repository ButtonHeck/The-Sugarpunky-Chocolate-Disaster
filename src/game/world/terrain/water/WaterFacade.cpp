#include "WaterFacade.h"

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

void WaterFacade::draw(bool useCulling, bool useDebugRender, glm::mat4& projectionView, glm::vec3 &viewPosition, Frustum &viewFrustum)
{
  shaders.update(useCulling, projectionView, viewPosition, viewFrustum);
  shaders.debugRenderMode(false);
  renderer.render(useCulling);

  if (useDebugRender)
    {
      shaders.debugRenderMode(true);
      renderer.debugRender(GL_TRIANGLES);
      shaders.updateNormals(projectionView);
      renderer.debugRender(GL_POINTS);
    }
}

void WaterFacade::bufferNewData()
{
  generator.bufferNewData();
}

void WaterFacade::updateAnimationFrame(double time, Options &options)
{
  generator.updateAnimationFrame(time, options);
}

const map2D_f &WaterFacade::getMap() const
{
  return generator.getMap();
}
